#include "Route.h"

#include <cmath>
#include <sstream>
#include <unordered_set>
#include <utility>

GraphWeight operator+(const GraphWeight& lhs, const GraphWeight& rhs) {
  return { lhs.wait_time + rhs.wait_time, lhs.bus_time + rhs.bus_time };
}

bool operator<(const GraphWeight& lhs, const GraphWeight& rhs) {
  return (lhs.wait_time + lhs.bus_time) < (rhs.wait_time + rhs.bus_time);
}

bool operator>(const GraphWeight& lhs, const GraphWeight& rhs) {
  return (lhs.wait_time + lhs.bus_time) > (rhs.wait_time + rhs.bus_time);
}

bool operator>=(const GraphWeight& lhs, int rhs) {
  return (lhs.wait_time + lhs.bus_time) >= rhs;
}

void Route::AddStop(std::string name, const StopsStorage& stops_storage) {
  Stop new_stop;
  new_stop.name = std::move(name);
  new_stop.stop_info = stops_storage.GetStopInfo(new_stop.name);
  m_stops.push_back(std::move(new_stop));
  m_unique_stops.insert(m_stops.back().name);
}

std::size_t Route::GetStopsOnRoute() const {
  switch (m_type)
  {
  case RouteType::LINEAR:
    return 2 * m_stops.size() - 1;
  case RouteType::CIRCLE:
    return m_stops.size();
  case RouteType::UNKNOWN:
    throw std::runtime_error("RouteType::UNKNOWN");
  }
  return 0;
}

std::size_t Route::GetUniqueStops() const {
  return m_unique_stops.size();
}

void Route::SetType(RouteType type) {
  m_type = type;

  switch (m_type)
  {
  case RouteType::LINEAR:
    ComputeDistanceLinearGeo();
    ComputeDistanceLinearRoad();
    break;
  case RouteType::CIRCLE:
    ComputeDistanceCircleGeo();
    ComputeDistanceCircleRoad();
    break;
  case RouteType::UNKNOWN:
    throw std::runtime_error("RouteType::UNKNOWN");
  }
}

RouteType Route::GetType() const {
  return m_type;
}

int Route::GetDistance() const {
  return m_distance_road;
}

double Route::GetCurvature() const {
  return m_distance_road / m_distance_geo;
}

void Route::SetRoutingSettings(RoutingSettings routing_settings) {
  m_routing_settings = routing_settings;
}

void Route::SetBusName(const std::string& bus_name) {
  m_bus_name = bus_name;
}

void Route::BuildGraph(Graph::DirectedWeightedGraph<GraphWeight>& graph) const {
  switch (m_type) {
  case RouteType::LINEAR:
    ComputeGraph(m_stops.cbegin(), m_stops.cend(), graph);
    ComputeGraph(m_stops.crbegin(), m_stops.crend(), graph);
    break;
  case RouteType::CIRCLE:
    ComputeGraph(m_stops.cbegin(), m_stops.cend(), graph);
    break;
  case RouteType::UNKNOWN:
    throw std::runtime_error("RouteType::UNKNOWN");
  }
}

const std::vector<Stop>& Route::GetStops() const {
  return m_stops;
}

bool Route::IsEndedEqual() const {
  return m_stops.at(0).name == m_stops.at(m_stops.size() - 1).name;
}

void Route::UpdatePaintCoords(const StopsStorage& stops_storage) {
  for (auto& stop : m_stops) {
    stop.stop_info.geo_coords.paint_lat = stops_storage.GetStopInfo(stop.name).geo_coords.paint_lat;
    stop.stop_info.geo_coords.paint_lon = stops_storage.GetStopInfo(stop.name).geo_coords.paint_lon;
  }
}

void Route::ComputeDistanceCircleGeo() {
  m_distance_geo = 0.0;
  for (size_t i = 1; i < m_stops.size(); ++i) {
    m_distance_geo += ComputeDistanceBetweenStops(m_stops[i - 1], m_stops[i]);
  }
}

void Route::ComputeDistanceLinearGeo() {
  m_distance_geo = 0.0;
  for (size_t i = 1; i < m_stops.size(); ++i) {
    m_distance_geo += 2 * ComputeDistanceBetweenStops(m_stops[i - 1], m_stops[i]);
  }
}

void Route::ComputeDistanceCircleRoad() {
  m_distance_road = ComputeDistanceRoad(m_stops.begin(), m_stops.end());
}

void Route::ComputeDistanceLinearRoad() {
  // from stop1 to stopN
  m_distance_road = ComputeDistanceRoad(m_stops.begin(), m_stops.end());

  // from stopN to stop1
  m_distance_road += ComputeDistanceRoad(m_stops.rbegin(), m_stops.rend());
}

double Route::ComputeDistanceBetweenStops(const Stop& lhs, const Stop& rhs) {
  using namespace std;
  static double pi = 3.1415926535;
  double lhs_lat = lhs.stop_info.geo_coords.latitude * pi / 180;
  double lhs_lon = lhs.stop_info.geo_coords.longitude * pi / 180;
  double rhs_lat = rhs.stop_info.geo_coords.latitude * pi / 180;
  double rhs_lon = rhs.stop_info.geo_coords.longitude * pi / 180;
  return acos(sin(lhs_lat) * sin(rhs_lat) + cos(lhs_lat) * cos(rhs_lat) * cos(abs(lhs_lon - rhs_lon))) * 6371000;
}
