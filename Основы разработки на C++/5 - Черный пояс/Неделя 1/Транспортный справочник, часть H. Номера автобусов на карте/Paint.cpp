#include <algorithm>
#include "Paint.h"

Paint::Paint(const RenderSettings& render_settings, const StopsStorage& stop_storage, const std::map<std::string, Bus>& buses):
  m_render_settings(render_settings), m_stop_storage(stop_storage), m_buses(buses)
{
  m_zoom_coef = ZoomCoef();
}

void Paint::Draw(std::ostream& out)
{
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

  color_idx = 0;
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

  for (const auto& [_, stop] : m_stop_storage.GetStops())
  {
    double cx = GetX(stop.geo_coords.longitude);
    double cy = GetY(stop.geo_coords.latitude);
    Svg::Circle circle;
    circle.SetCenter(Svg::Point{ cx, cy }).SetRadius(m_render_settings.stop_radius).SetFillColor("white");
    m_svg_doc.Add(circle);
  }

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

  m_svg_doc.Render(out);
}

double Paint::WidthZoomCoef() const {
  return (m_render_settings.width - 2 * m_render_settings.padding) /
    (m_stop_storage.max_lon.value() - m_stop_storage.min_lon.value());
}

double Paint::HeightZoomCoef() const {
  return (m_render_settings.height - 2 * m_render_settings.padding) /
    (m_stop_storage.max_lat.value() - m_stop_storage.min_lat.value());
}

double Paint::ZoomCoef() const {
  if (m_stop_storage.max_lon.value() - m_stop_storage.min_lon.value() == 0 &&
      m_stop_storage.max_lat.value() - m_stop_storage.min_lat.value() == 0) {
    return 0.0;
  }
  else if (m_stop_storage.max_lon.value() - m_stop_storage.min_lon.value() == 0) {
    return HeightZoomCoef();
  }
  else if (m_stop_storage.max_lat.value() - m_stop_storage.min_lat.value() == 0) {
    return WidthZoomCoef();
  }
  else {
    return std::min(WidthZoomCoef(), HeightZoomCoef());
  }
}

double Paint::GetX(double longitude) const {
  return (longitude - m_stop_storage.min_lon.value())* m_zoom_coef + m_render_settings.padding;
}

double Paint::GetY(double latitude) const {
  return (m_stop_storage.max_lat.value() - latitude) * m_zoom_coef + m_render_settings.padding;
}
