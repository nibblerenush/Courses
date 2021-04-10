#pragma once

#include "Bus.h"
#include "StopsStorage.h"
#include "Svg.h"

#include <functional>

struct RenderSettings
{
  double width = 0;
  double height = 0;
  double padding = 0;
  double stop_radius = 0;
  double line_width = 0;
  int stop_label_font_size = 0;
  Svg::Point stop_label_offset;
  Svg::Color underlayer_color;
  double underlayer_width = 0;
  std::vector<Svg::Color> color_palette;
  int bus_label_font_size = 0;
  Svg::Point bus_label_offset;
  std::vector<std::string> layers;
};

class Paint
{
public:
  Paint(const RenderSettings& render_settings, const StopsStorage& stop_storage, const std::map<std::string, Bus>& buses);
  void Draw(std::ostream& out);

private:
  double WidthZoomCoef() const;
  double HeightZoomCoef() const;
  double ZoomCoef() const;
  double GetX(double longitude) const;
  double GetY(double latitude) const;
  void DrawBusLines();
  void DrawBusLabels();
  void DrawStopPoints();
  void DrawStopLabels();

private:
  const RenderSettings& m_render_settings;
  const StopsStorage& m_stop_storage;
  const std::map<std::string, Bus>& m_buses;

  double m_zoom_coef;
  Svg::Document m_svg_doc;
  const std::unordered_map<std::string, std::function<void()>> m_draw_functions =
  {
    {"bus_lines", std::bind(&Paint::DrawBusLines, this)},
    {"bus_labels", std::bind(&Paint::DrawBusLabels, this)},
    {"stop_points", std::bind(&Paint::DrawStopPoints, this)},
    {"stop_labels", std::bind(&Paint::DrawStopLabels, this)}
  };
};
