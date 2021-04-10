#pragma once

#include "Bus.h"
#include "StopsStorage.h"
#include "Svg.h"

struct RenderSettings
{
  double width;
  double height;
  double padding;
  double stop_radius;
  double line_width;
  int stop_label_font_size;
  Svg::Point stop_label_offset;
  Svg::Color underlayer_color;
  double underlayer_width;
  std::vector<Svg::Color> color_palette;
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

private:
  const RenderSettings& m_render_settings;
  const StopsStorage& m_stop_storage;
  const std::map<std::string, Bus>& m_buses;

  double m_zoom_coef;
  Svg::Document m_svg_doc;
};
