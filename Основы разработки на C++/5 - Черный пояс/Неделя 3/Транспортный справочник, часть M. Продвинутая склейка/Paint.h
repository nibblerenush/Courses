#pragma once

#include "Bus.h"
#include "StopsStorage.h"
#include "Svg.h"

#include <functional>
#include <map>

struct RenderSettings {
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

class Paint {
public:
  Paint(const RenderSettings& render_settings, const StopsStorage& stop_storage, const std::map<std::string, Bus>& buses);
  void Render(std::ostream& out);

private:
  struct LonLatIdx {
    std::size_t idx = 0;
    std::map<std::string, StopInfo>::const_iterator stop;
  };

  void DefineReferenceStops();
  void CalcPaintGeoCoords();

  void CreateLonLatIdx();
  void CalcXyStep();

  double GetX(double paint_lon) const;
  double GetY(double paint_lat) const;

  void Draw();
  void DrawBusLines();
  void DrawBusLabels();
  void DrawStopPoints();
  void DrawStopLabels();

  bool IsRouteNeighbors(
    std::map<std::string, StopInfo>::const_iterator f_stop,
    std::map<std::string, StopInfo>::const_iterator s_stop) const;
  void SetIdx(std::map<double, LonLatIdx>& idx_map, std::size_t& max_idx);

private:
  const RenderSettings& m_render_settings;
  const StopsStorage& m_stop_storage;
  const std::map<std::string, Bus>& m_buses;
  
  Svg::Document m_svg_doc;
  const std::unordered_map<std::string, std::function<void()>> m_draw_functions = {
    {"bus_lines", std::bind(&Paint::DrawBusLines, this)},
    {"bus_labels", std::bind(&Paint::DrawBusLabels, this)},
    {"stop_points", std::bind(&Paint::DrawStopPoints, this)},
    {"stop_labels", std::bind(&Paint::DrawStopLabels, this)}
  };

  std::map<double, LonLatIdx> m_lon_idx;
  std::map<double, LonLatIdx> m_lat_idx;
  double m_x_step = 0.0;
  std::size_t m_max_lon_idx = 0;
  double m_y_step = 0.0;
  std::size_t m_max_lat_idx = 0;

  std::unordered_map<std::string, bool> m_reference_stops;
};
