#include <algorithm>
#include "Paint.h"

Paint::Paint(const RenderSettings& render_settings, const StopsStorage& stop_storage, const std::map<std::string, Bus>& buses):
m_render_settings(render_settings), m_stop_storage(stop_storage), m_buses(buses) {
  CreateLonLatIdx();
  CalcXyStep();
  Draw();
}

void Paint::Render(std::ostream& out) {
  m_svg_doc.Render(out);
}

void Paint::Draw() {
  for (const auto& layer : m_render_settings.layers) {
    m_draw_functions.at(layer)();
  }
}

void Paint::CreateLonLatIdx() {
  for (auto iter = m_stop_storage.GetStops().cbegin(); iter != m_stop_storage.GetStops().cend(); ++iter) {
    m_lon_idx[iter->second.geo_coords.longitude] = { 0, iter };
    m_lat_idx[iter->second.geo_coords.latitude] = { 0, iter };
  }
  
  SetIdx(m_lon_idx, m_max_lon_idx);
  SetIdx(m_lat_idx, m_max_lat_idx);
}

void Paint::CalcXyStep() {
  if (m_max_lon_idx > 0) {
    m_x_step = (m_render_settings.width - 2 * m_render_settings.padding) / m_max_lon_idx;
  }

  if (m_max_lat_idx > 0) {
    m_y_step = (m_render_settings.height - 2 * m_render_settings.padding) / m_max_lat_idx;
  }
}

double Paint::GetX(double longitude) const {
  return m_lon_idx.at(longitude).idx * m_x_step + m_render_settings.padding;
}

double Paint::GetY(double latitude) const {
  return m_render_settings.height - m_render_settings.padding - m_lat_idx.at(latitude).idx * m_y_step;
}

void Paint::DrawBusLines() {
  std::size_t color_idx = 0;
  std::size_t color_size = m_render_settings.color_palette.size();
  for (const auto& [_, bus] : m_buses)
  {
    if (color_idx == color_size) {
      color_idx = 0;
    }

    Svg::Polyline polyline;
    polyline
      .SetStrokeColor(m_render_settings.color_palette[color_idx])
      .SetStrokeWidth(m_render_settings.line_width)
      .SetStrokeLineCap("round")
      .SetStrokeLineJoin("round");

    for (const auto& stop : bus.GetRoute().GetStops())
    {
      double x = GetX(stop.stop_info.geo_coords.longitude);
      double y = GetY(stop.stop_info.geo_coords.latitude);
      polyline.AddPoint(Svg::Point(x, y));
    }

    if (bus.GetRoute().GetType() == RouteType::LINEAR)
    {
      bool is_first = true;
      for (auto iter = bus.GetRoute().GetStops().rbegin(); iter != bus.GetRoute().GetStops().rend(); ++iter)
      {
        if (is_first) {
          is_first = false;
        }
        else {
          double x = GetX(iter->stop_info.geo_coords.longitude);
          double y = GetY(iter->stop_info.geo_coords.latitude);
          polyline.AddPoint(Svg::Point(x, y));
        }
      }
    }

    m_svg_doc.Add(polyline);
    color_idx++;
  }
}

void Paint::DrawBusLabels() {
  std::size_t color_idx = 0;
  std::size_t color_size = m_render_settings.color_palette.size();
  for (const auto& item : m_buses)
  {
    if (color_idx == color_size) {
      color_idx = 0;
    }

    auto common_settings = [this, &item](Svg::Text& text, double longitude, double latitude)
    {
      double x = GetX(longitude);
      double y = GetY(latitude);

      text
        .SetPoint(Svg::Point{ x, y })
        .SetOffset(m_render_settings.bus_label_offset)
        .SetFontSize(m_render_settings.bus_label_font_size)
        .SetFontFamily("Verdana")
        .SetFontWeight("bold")
        .SetData(item.first);
    };

    Svg::Text start_bus_substrait;
    Svg::Text start_bus_text;

    auto set_bus_number = [this, &item, common_settings, color_idx](Svg::Text& bus_substrait, Svg::Text& bus_text, std::size_t stop_number)
    {
      common_settings(bus_substrait,
        item.second.GetRoute().GetStops().at(stop_number).stop_info.geo_coords.longitude,
        item.second.GetRoute().GetStops().at(stop_number).stop_info.geo_coords.latitude);

      common_settings(bus_text,
        item.second.GetRoute().GetStops().at(stop_number).stop_info.geo_coords.longitude,
        item.second.GetRoute().GetStops().at(stop_number).stop_info.geo_coords.latitude);

      bus_substrait
        .SetFillColor(m_render_settings.underlayer_color)
        .SetStrokeColor(m_render_settings.underlayer_color)
        .SetStrokeWidth(m_render_settings.underlayer_width)
        .SetStrokeLineCap("round")
        .SetStrokeLineJoin("round");

      bus_text.SetFillColor(m_render_settings.color_palette[color_idx]);
    };

    set_bus_number(start_bus_substrait, start_bus_text, 0);
    m_svg_doc.Add(start_bus_substrait);
    m_svg_doc.Add(start_bus_text);

    if (item.second.GetRoute().GetType() == RouteType::LINEAR && !item.second.GetRoute().IsEndedEqual())
    {
      Svg::Text finish_bus_substrait;
      Svg::Text finish_bus_text;

      set_bus_number(finish_bus_substrait, finish_bus_text, item.second.GetRoute().GetStops().size() - 1);
      m_svg_doc.Add(finish_bus_substrait);
      m_svg_doc.Add(finish_bus_text);
    }

    color_idx++;
  }
}

void Paint::DrawStopPoints() {
  for (const auto& [_, stop] : m_stop_storage.GetStops()) {
    double cx = GetX(stop.geo_coords.longitude);
    double cy = GetY(stop.geo_coords.latitude);
    Svg::Circle circle;
    
    circle.SetCenter(Svg::Point{ cx, cy }).SetRadius(m_render_settings.stop_radius).SetFillColor("white");
    m_svg_doc.Add(circle);
  }
}

void Paint::DrawStopLabels() {
  for (const auto& item : m_stop_storage.GetStops())
  {
    auto common_settings = [this, &item](Svg::Text& text)
    {
      double x = GetX(item.second.geo_coords.longitude);
      double y = GetY(item.second.geo_coords.latitude);
      text
        .SetPoint(Svg::Point{ x, y })
        .SetOffset(m_render_settings.stop_label_offset)
        .SetFontSize(m_render_settings.stop_label_font_size)
        .SetFontFamily("Verdana")
        .SetData(item.first);
    };

    Svg::Text stop_substrait;
    Svg::Text stop_text;

    common_settings(stop_substrait);
    common_settings(stop_text);

    stop_substrait
      .SetFillColor(m_render_settings.underlayer_color)
      .SetStrokeColor(m_render_settings.underlayer_color)
      .SetStrokeWidth(m_render_settings.underlayer_width)
      .SetStrokeLineCap("round")
      .SetStrokeLineJoin("round");

    stop_text.SetFillColor("black");

    m_svg_doc.Add(stop_substrait);
    m_svg_doc.Add(stop_text);
  }
}

bool Paint::IsRouteNeighbors(
  std::map<std::string, StopInfo>::const_iterator f_stop,
  std::map<std::string, StopInfo>::const_iterator s_stop) const
{
  std::vector<std::string> common_buses;
  std::set_intersection(
    f_stop->second.buses.begin(), f_stop->second.buses.end(),
    s_stop->second.buses.begin(), s_stop->second.buses.end(),
    std::back_inserter(common_buses));
  
  for (const std::string& bus_name : common_buses) {
    const Route& route = m_buses.at(bus_name).GetRoute();
    const std::vector<Stop>& stops = route.GetStops();
    
    for (auto iter = stops.begin(); iter != std::prev(stops.end()); ++iter) {
      if (iter->name == f_stop->first && std::next(iter)->name == s_stop->first) {
        return true;
      }
      else if (iter->name == s_stop->first && std::next(iter)->name == f_stop->first) {
        return true;
      }
    }
  }
  
  return false;
}

void Paint::SetIdx(std::map<double, LonLatIdx>& idx_map, std::size_t& max_idx) {
  std::size_t idx = 0;
  auto border = idx_map.begin();
  for (auto iter = next(border); iter != idx_map.end(); ++iter) {
    bool is_same_idx = true;
    for (auto iter2 = border; iter2 != iter; ++iter2) {
      if (IsRouteNeighbors(iter->second.stop, iter2->second.stop)) {
        is_same_idx = false;
        break;
      }
    }

    if (is_same_idx) {
      iter->second.idx = idx;
    }
    else {
      iter->second.idx = idx + 1;
      idx++;
      border = iter;
    }
  }
  max_idx = idx;
}
