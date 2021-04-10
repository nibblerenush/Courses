#pragma once

#include "Bus.h"
#include "StopsStorage.h"
#include "Svg.h"

#include <functional>
#include <map>
#include <string_view>

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
  double outer_margin = 0;
};

struct TimeElement {
  std::string stop_name;
};

using stop_iter = std::vector<Stop>::const_iterator;

struct BusElement {
  std::string bus_name;
  stop_iter from;
  stop_iter to;
};

class Paint {
public:
  Paint(const RenderSettings& render_settings, const StopsStorage& stop_storage, const std::map<std::string, Bus>& buses);
  void Render(std::ostream& out);
  void RenderRoute(
    std::ostream& out,
    const std::vector<BusElement>& bus_elems,
    const std::vector<TimeElement>& time_elems);
    
private:
  struct LonLatIdx {
    std::size_t idx = 0;
    std::map<std::string, StopInfo>::const_iterator stop;
  };
  
  // 1 step
  void DefineReferenceStops();

  // 2 step
  void CalcPaintGeoCoords();

  // 3 step
  void CreateLonLatIdx();
  void SetIdx(std::map<double, LonLatIdx>& idx_map, std::size_t& max_idx);
  bool IsRouteNeighbors(
    std::map<std::string, StopInfo>::const_iterator f_stop,
    std::map<std::string, StopInfo>::const_iterator s_stop) const;

  // 4 step
  void CalcXyStep();

  // 5 step
  void CalcBusColor();

  double GetX(double paint_lon) const;
  double GetY(double paint_lat) const;

  void Draw();

  // Bus lines
  void DrawBusLines();
  void DrawRouteBusLines(
    Svg::Document& svg_route,
    const std::vector<BusElement>& bus_elems,
    const std::vector<TimeElement>& time_elems);

  // Bus labels
  void DrawBusLabel(
    const std::string& bus_name,
    std::size_t stop_number,
    Svg::Text& bus_substrait,
    Svg::Text& bus_text);
  void DrawBusLabels();
  void DrawRouteBusLabels(
    Svg::Document& svg_route,
    const std::vector<BusElement>& bus_elems,
    const std::vector<TimeElement>& time_elems);

  // Stop points
  void DrawStopPoints();
  void DrawRouteStopPoints(
    Svg::Document& svg_route,
    const std::vector<BusElement>& bus_elems,
    const std::vector<TimeElement>& time_elems);

  // Stop labels
  void DrawStopLabel(
    const std::string& stop_name,
    Svg::Text& stop_substrait,
    Svg::Text& stop_text);
  void DrawStopLabels();
  void DrawRouteStopLabels(
    Svg::Document& svg_route,
    const std::vector<BusElement>& bus_elems,
    const std::vector<TimeElement>& time_elems);

private:
  // Common
  const RenderSettings& m_render_settings;
  const StopsStorage& m_stop_storage;
  const std::map<std::string, Bus>& m_buses;
  
  // Draw map
  Svg::Document m_svg_all_map;
  const std::unordered_map<std::string, std::function<void()>> m_map_draw_functions = {
    {"bus_lines", std::bind(&Paint::DrawBusLines, this)},
    {"bus_labels", std::bind(&Paint::DrawBusLabels, this)},
    {"stop_points", std::bind(&Paint::DrawStopPoints, this)},
    {"stop_labels", std::bind(&Paint::DrawStopLabels, this)}
  };
  
  // Draw route
  const std::unordered_map<
        std::string,
        std::function<void(Svg::Document&, const std::vector<BusElement>&, const std::vector<TimeElement>& time_elems)>
        > m_route_draw_functions = {
    {"bus_lines", std::bind(&Paint::DrawRouteBusLines,
      this,
      std::placeholders::_1,
      std::placeholders::_2,
      std::placeholders::_3)},

    {"bus_labels", std::bind(&Paint::DrawRouteBusLabels, this,
      std::placeholders::_1,
      std::placeholders::_2,
      std::placeholders::_3)},

    {"stop_points", std::bind(&Paint::DrawRouteStopPoints, this,
      std::placeholders::_1,
      std::placeholders::_2,
      std::placeholders::_3)},

    {"stop_labels", std::bind(&Paint::DrawRouteStopLabels, this,
      std::placeholders::_1,
      std::placeholders::_2,
      std::placeholders::_3)}
  };

  // Geo info
  std::map<double, LonLatIdx> m_lon_idx;
  std::map<double, LonLatIdx> m_lat_idx;
  double m_x_step = 0.0;
  std::size_t m_max_lon_idx = 0;
  double m_y_step = 0.0;
  std::size_t m_max_lat_idx = 0;

  // Help info
  std::unordered_map<std::string, bool> m_reference_stops;
  std::unordered_map<std::string_view, Svg::Color> m_bus_color;
};
