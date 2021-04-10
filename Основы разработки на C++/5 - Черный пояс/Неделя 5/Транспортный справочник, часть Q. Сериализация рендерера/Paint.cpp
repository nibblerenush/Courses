#include <algorithm>
#include "Paint.h"

Paint::Paint(const RenderSettings& render_settings, const StopsStorage& stop_storage, const std::map<std::string, Bus>& buses):
m_render_settings(render_settings), m_stop_storage(stop_storage), m_buses(buses)
{
  // 1 step
  DefineReferenceStops();

  // 2 step
  CalcPaintGeoCoords();

  // 3 step
  CreateLonLatIdx();

  // 4 step
  CalcXyStep();

  // 5 step
  CalcBusColor();

  // Draw map
  Draw();
}

void Paint::Render(std::ostream& out) {
  m_svg_all_map.Render(out);
}

void Paint::RenderRoute(
  std::ostream& out,
  const std::vector<BusElement>& bus_elems,
  const std::vector<TimeElement>& time_elems)
{
  // 1 step
  Svg::Document svg_route = m_svg_all_map;

  // 2 step
  svg_route.Add(Svg::Rectangle{}
    .SetLeftCorner(Svg::Point{ -m_render_settings.outer_margin, -m_render_settings.outer_margin })
    .SetWidth(m_render_settings.width + 2 * m_render_settings.outer_margin)
    .SetHeight(m_render_settings.height + 2 * m_render_settings.outer_margin)
    .SetFillColor(m_render_settings.underlayer_color)
  );

  // 3 step
  for (const auto& layer : m_render_settings.layers) {
    m_route_draw_functions.at(layer)(svg_route, bus_elems, time_elems);
  }

  svg_route.Render(out);
}

// ========== Preparation ==========

void Paint::DefineReferenceStops() {
  // More than one bus or not buses
  const std::map<std::string, StopInfo>& all_stops = m_stop_storage.GetStops();
  for (const auto& [name, stop_info] : all_stops) {
    if (stop_info.buses.size() > 1 || stop_info.buses.empty()) {
      m_reference_stops[name] = true;
    }
    else {
      m_reference_stops[name] = false;
    }
  }

  // End stops or more than two on route
  for (const auto& [_, bus] : m_buses) {
    const Route& route = bus.GetRoute();
    const std::vector<Stop>& stops = route.GetStops();

    std::unordered_map<std::string, int> stop_count;
    for (std::size_t i = 0; i < stops.size(); ++i) {
      if (i == 0 || i == stops.size() - 1) {
        m_reference_stops[stops[i].name] = true;
      }
      else {
        if (route.GetType() == RouteType::LINEAR) {
          stop_count[stops[i].name] = stop_count[stops[i].name] + 2;
        }
        else {
          stop_count[stops[i].name] = stop_count[stops[i].name] + 1;
        }
      }
    }

    for (const auto& [stop_name, count] : stop_count) {
      if (count > 2) {
        m_reference_stops[stop_name] = true;
      }
    }
  }
}

void Paint::CalcPaintGeoCoords() {
  StopsStorage& stop_storage = const_cast<StopsStorage&>(m_stop_storage);
  
  auto UpdatePaintCoords = [&stop_storage](const std::vector<Stop>& stops, std::size_t i, std::size_t j) {
    double lon_step = (stops[j].stop_info.geo_coords.longitude - stops[i].stop_info.geo_coords.longitude) / (j - i);
    double lat_step = (stops[j].stop_info.geo_coords.latitude - stops[i].stop_info.geo_coords.latitude) / (j - i);
    for (std::size_t k = i + 1; k <= j - 1; ++k) {
      double paint_lon = stops[i].stop_info.geo_coords.longitude + lon_step * (k - i);
      double paint_lat = stops[i].stop_info.geo_coords.latitude + lat_step * (k - i);
      stop_storage.UpdatePaintCoords(stops[k].name, paint_lon, paint_lat);
    }
  };

  for (const auto& [_, bus] : m_buses) {
    Route& route = const_cast<Route&>(bus.GetRoute());
    const std::vector<Stop>& stops = route.GetStops();
    
    std::size_t i = 0;
    std::size_t j = i + 1;
    while (j < stops.size()) {
      if (m_reference_stops[stops[j].name]) {
        UpdatePaintCoords(stops, i, j);
        i = j;
      }
      j++;
    }
    UpdatePaintCoords(stops, i, j - 1);
    route.UpdatePaintCoords(stop_storage);
  }
}

void Paint::CreateLonLatIdx() {
  for (auto iter = m_stop_storage.GetStops().cbegin(); iter != m_stop_storage.GetStops().cend(); ++iter) {
    m_lon_idx[iter->second.geo_coords.paint_lon] = { 0, iter };
    m_lat_idx[iter->second.geo_coords.paint_lat] = { 0, iter };
  }
  
  SetIdx(m_lon_idx, m_max_lon_idx);
  SetIdx(m_lat_idx, m_max_lat_idx);
}

void Paint::SetIdx(std::map<double, LonLatIdx>& idx_map, std::size_t& max_idx) {
  for (auto iter = next(idx_map.begin()); iter != idx_map.end(); ++iter) {
    bool is_zero_idx = true;
    for (auto iter2 = idx_map.begin(); iter2 != iter; ++iter2) {
      if (IsRouteNeighbors(iter->second.stop, iter2->second.stop)) {
        is_zero_idx = false;
        break;
      }
    }
    
    if (!is_zero_idx) {
      std::size_t new_idx = 0;
      for (auto iter2 = idx_map.begin(); iter2 != iter; ++iter2) {
        if (IsRouteNeighbors(iter->second.stop, iter2->second.stop) && iter2->second.idx > new_idx) {
          new_idx = iter2->second.idx;
        }
      }
      iter->second.idx = new_idx + 1;

      if (iter->second.idx > max_idx) {
        max_idx = iter->second.idx;
      }
    }
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


void Paint::CalcXyStep() {
  if (m_max_lon_idx > 0) {
    m_x_step = (m_render_settings.width - 2 * m_render_settings.padding) / m_max_lon_idx;
  }

  if (m_max_lat_idx > 0) {
    m_y_step = (m_render_settings.height - 2 * m_render_settings.padding) / m_max_lat_idx;
  }
}

void Paint::CalcBusColor() {
  std::size_t color_idx = 0;
  std::size_t color_size = m_render_settings.color_palette.size();
  for (const auto& [name, _] : m_buses) {
    if (color_idx == color_size) {
      color_idx = 0;
    }
    m_bus_color[name] = m_render_settings.color_palette[color_idx];
    color_idx++;
  }
}

// ========== Preparation ==========

double Paint::GetX(double paint_lon) const {
  return m_lon_idx.at(paint_lon).idx * m_x_step + m_render_settings.padding;
}

double Paint::GetY(double paint_lat) const {
  return m_render_settings.height - m_render_settings.padding - m_lat_idx.at(paint_lat).idx * m_y_step;
}

// ========== Draw ==========

void Paint::Draw() {
  for (const auto& layer : m_render_settings.layers) {
    m_map_draw_functions.at(layer)();
  }
}

void Paint::DrawBusLines() {
  for (const auto& [name, bus] : m_buses)
  {
    Svg::Polyline polyline;
    polyline
      .SetStrokeColor(m_bus_color[name])
      .SetStrokeWidth(m_render_settings.line_width)
      .SetStrokeLineCap("round")
      .SetStrokeLineJoin("round");

    for (const auto& stop : bus.GetRoute().GetStops())
    {
      double x = GetX(stop.stop_info.geo_coords.paint_lon);
      double y = GetY(stop.stop_info.geo_coords.paint_lat);
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
          double x = GetX(iter->stop_info.geo_coords.paint_lon);
          double y = GetY(iter->stop_info.geo_coords.paint_lat);
          polyline.AddPoint(Svg::Point(x, y));
        }
      }
    }

    m_svg_all_map.Add(polyline);
  }
}

void Paint::DrawRouteBusLines(
  Svg::Document& svg_route,
  const std::vector<BusElement>& bus_elems,
  const std::vector<TimeElement>&)
{
  auto AddPoints = [this](Svg::Polyline& polyline, auto start, auto finish) {
    for (auto iter = start; iter <= finish; ++iter) {
      double x = GetX(iter->stop_info.geo_coords.paint_lon);
      double y = GetY(iter->stop_info.geo_coords.paint_lat);
      polyline.AddPoint(Svg::Point(x, y));
    }
  };

  for (const auto& bus_elem : bus_elems) {
    Svg::Polyline polyline;
    polyline
      .SetStrokeColor(m_bus_color[bus_elem.bus_name])
      .SetStrokeWidth(m_render_settings.line_width)
      .SetStrokeLineCap("round")
      .SetStrokeLineJoin("round");

    if (bus_elem.from < bus_elem.to) {
      AddPoints(polyline, bus_elem.from, bus_elem.to);
    }
    else {
      AddPoints(polyline,
        std::vector<Stop>::const_reverse_iterator(bus_elem.from) - 1,
        std::vector<Stop>::const_reverse_iterator(bus_elem.to) - 1);
    }
    
    svg_route.Add(std::move(polyline));
  }
}

void Paint::DrawBusLabel(
  const std::string& bus_name,
  std::size_t stop_number,
  Svg::Text& bus_substrait,
  Svg::Text& bus_text)
{
  auto common_settings = [this, &bus_name](Svg::Text& text, double lon, double lat)
  {
    double x = GetX(lon);
    double y = GetY(lat);
    
    text
      .SetPoint(Svg::Point{ x, y })
      .SetOffset(m_render_settings.bus_label_offset)
      .SetFontSize(m_render_settings.bus_label_font_size)
      .SetFontFamily("Verdana")
      .SetFontWeight("bold")
      .SetData(bus_name);
  };
  
  common_settings(bus_substrait,
    m_buses.at(bus_name).GetRoute().GetStops().at(stop_number).stop_info.geo_coords.paint_lon,
    m_buses.at(bus_name).GetRoute().GetStops().at(stop_number).stop_info.geo_coords.paint_lat);
    
  common_settings(bus_text,
    m_buses.at(bus_name).GetRoute().GetStops().at(stop_number).stop_info.geo_coords.paint_lon,
    m_buses.at(bus_name).GetRoute().GetStops().at(stop_number).stop_info.geo_coords.paint_lat);
    
  bus_substrait
    .SetFillColor(m_render_settings.underlayer_color)
    .SetStrokeColor(m_render_settings.underlayer_color)
    .SetStrokeWidth(m_render_settings.underlayer_width)
    .SetStrokeLineCap("round")
    .SetStrokeLineJoin("round");
    
  bus_text.SetFillColor(m_bus_color[bus_name]);
}

void Paint::DrawBusLabels() {
  for (const auto& [bus_name, bus] : m_buses)
  {
    Svg::Text start_bus_substrait;
    Svg::Text start_bus_text;
    
    DrawBusLabel(bus_name, 0, start_bus_substrait, start_bus_text);
    m_svg_all_map.Add(std::move(start_bus_substrait));
    m_svg_all_map.Add(std::move(start_bus_text));

    if (bus.GetRoute().GetType() == RouteType::LINEAR && !bus.GetRoute().IsEndedEqual())
    {
      Svg::Text finish_bus_substrait;
      Svg::Text finish_bus_text;

      DrawBusLabel(bus_name, bus.GetRoute().GetStops().size() - 1, finish_bus_substrait, finish_bus_text);
      m_svg_all_map.Add(std::move(finish_bus_substrait));
      m_svg_all_map.Add(std::move(finish_bus_text));
    }
  }
}

void Paint::DrawRouteBusLabels(
  Svg::Document& svg_route,
  const std::vector<BusElement>& bus_elems,
  const std::vector<TimeElement>&)
{
  auto check_stop = [this, &svg_route](const std::string& bus_name, stop_iter stop) {
    if (stop->name == m_buses.at(bus_name).GetRoute().GetStops().cbegin()->name ||
        stop->name == std::prev(m_buses.at(bus_name).GetRoute().GetStops().cend())->name)
    {
      Svg::Text bus_substrait;
      Svg::Text bus_text;
      
      DrawBusLabel(
        bus_name,
        stop - m_buses.at(bus_name).GetRoute().GetStops().cbegin(),
        bus_substrait,
        bus_text);
        
        svg_route.Add(std::move(bus_substrait));
        svg_route.Add(std::move(bus_text));
    }
  };

  for (const auto& bus_elem : bus_elems) {
    check_stop(bus_elem.bus_name, bus_elem.from);
    check_stop(bus_elem.bus_name, bus_elem.to);
  }
}

void Paint::DrawStopPoints() {
  for (const auto& [_, stop] : m_stop_storage.GetStops()) {
    double cx = GetX(stop.geo_coords.paint_lon);
    double cy = GetY(stop.geo_coords.paint_lat);
    Svg::Circle circle;
    
    circle.SetCenter(Svg::Point{ cx, cy }).SetRadius(m_render_settings.stop_radius).SetFillColor("white");
    m_svg_all_map.Add(std::move(circle));
  }
}

void Paint::DrawRouteStopPoints(
  Svg::Document& svg_route,
  const std::vector<BusElement>& bus_elems,
  const std::vector<TimeElement>&)
{
  auto AddCircle = [this](Svg::Document& svg_route, auto start, auto finish) {
    for (auto iter = start; iter <= finish; ++iter) {
      double cx = GetX(iter->stop_info.geo_coords.paint_lon);
      double cy = GetY(iter->stop_info.geo_coords.paint_lat);
      Svg::Circle circle;
      
      circle.SetCenter(Svg::Point{ cx, cy }).SetRadius(m_render_settings.stop_radius).SetFillColor("white");
      svg_route.Add(std::move(circle));
    }
  };

  for (const auto& bus_elem : bus_elems) {
    if (bus_elem.from < bus_elem.to) {
      AddCircle(svg_route, bus_elem.from, bus_elem.to);
    }
    else {
      AddCircle(svg_route,
        std::vector<Stop>::const_reverse_iterator(bus_elem.from) - 1,
        std::vector<Stop>::const_reverse_iterator(bus_elem.to) - 1);
    }
  }
}

void Paint::DrawStopLabel(
    const std::string& stop_name,
    Svg::Text& stop_substrait,
    Svg::Text& stop_text)
{
  auto common_settings = [this, &stop_name](Svg::Text& text)
  {
    double x = GetX(m_stop_storage.GetStopInfo(stop_name).geo_coords.paint_lon);
    double y = GetY(m_stop_storage.GetStopInfo(stop_name).geo_coords.paint_lat);
    
    text
      .SetPoint(Svg::Point{ x, y })
      .SetOffset(m_render_settings.stop_label_offset)
      .SetFontSize(m_render_settings.stop_label_font_size)
      .SetFontFamily("Verdana")
      .SetData(stop_name);
  };

  common_settings(stop_substrait);
  common_settings(stop_text);

  stop_substrait
      .SetFillColor(m_render_settings.underlayer_color)
      .SetStrokeColor(m_render_settings.underlayer_color)
      .SetStrokeWidth(m_render_settings.underlayer_width)
      .SetStrokeLineCap("round")
      .SetStrokeLineJoin("round");

  stop_text.SetFillColor("black");
}

void Paint::DrawStopLabels() {
  for (const auto& [name, _] : m_stop_storage.GetStops())
  {
    Svg::Text stop_substrait;
    Svg::Text stop_text;
    
    DrawStopLabel(name, stop_substrait, stop_text);
    m_svg_all_map.Add(std::move(stop_substrait));
    m_svg_all_map.Add(std::move(stop_text));
  }
}

void Paint::DrawRouteStopLabels(
    Svg::Document& svg_route,
    const std::vector<BusElement>& bus_elems,
    const std::vector<TimeElement>& time_elems)
{
  for (const auto& time_elem : time_elems) {
    Svg::Text stop_substrait;
    Svg::Text stop_text;
    
    DrawStopLabel(time_elem.stop_name, stop_substrait, stop_text);
    svg_route.Add(std::move(stop_substrait));
    svg_route.Add(std::move(stop_text));
  }

  // Last stop
  if (!bus_elems.empty()) {
    auto last_stop = std::prev(bus_elems.cend())->to->name;
    Svg::Text stop_substrait;
    Svg::Text stop_text;
    DrawStopLabel(last_stop, stop_substrait, stop_text);
    svg_route.Add(std::move(stop_substrait));
    svg_route.Add(std::move(stop_text));
  }
}
