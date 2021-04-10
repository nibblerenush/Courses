#include "Route.h"

#include <cmath>
#include <stdexcept>

GraphWeight::GraphWeight(int wt, double bt) : wait_time(wt), bus_time(bt)
{}

GraphWeight::GraphWeight(int zero): wait_time(0), bus_time(0.0) {
  if (zero != 0) {
    throw std::runtime_error("zero != 0");
  }
}

GraphWeight operator + (const GraphWeight& lhs, const GraphWeight& rhs) {
  return { lhs.wait_time + rhs.wait_time, lhs.bus_time + rhs.bus_time };
}

bool operator < (const GraphWeight& lhs, const GraphWeight& rhs) {
  return (lhs.wait_time + lhs.bus_time) < (rhs.wait_time + rhs.bus_time);
}

bool operator > (const GraphWeight& lhs, const GraphWeight& rhs) {
  return (lhs.wait_time + lhs.bus_time) > (rhs.wait_time + rhs.bus_time);
}

bool operator >= (const GraphWeight& lhs, int rhs) {
  return (lhs.wait_time + lhs.bus_time) >= rhs;
}

RoutingSettings Route::m_routing_settings;

Route::Route(const std::string& route_name) : m_route_name(route_name)
{}

void Route::AddStop(const std::string& stop_name, const StopsStorage& stops_storage) {
  auto iter = stops_storage.GetStops().find(stop_name);
  if (iter != stops_storage.GetStops().end()) {
    m_stops.push_back(Stop{iter->first, iter->second});
    m_unique_stops.insert(iter->first);
  }
  else {
    throw std::runtime_error("Route::AddStop");
  }
}

void Route::SetType(RouteType type) {
  m_type = type;

  switch (m_type) {
  case RouteType::LINEAR:
    ComputeDistanceLinearGeo();
    ComputeDistanceLinearRoad();
    break;
  case RouteType::CIRCLE:
    ComputeDistanceCircleGeo();
    ComputeDistanceCircleRoad();
    break;
  default:
    throw std::runtime_error("Route::SetType");
  }
}

RouteType Route::GetType() const {
  return m_type;
}

const std::string& Route::GetName() const {
  return m_route_name;
}

std::size_t Route::GetStopsOnRoute() const {
  switch (m_type) {
  case RouteType::LINEAR:
    return 2 * m_stops.size() - 1;
  case RouteType::CIRCLE:
    return m_stops.size();
  default:
    throw std::runtime_error("Route::GetStopsOnRoute");
  }
}

std::size_t Route::GetUniqueStops() const {
  return m_unique_stops.size();
}

int Route::GetDistance() const {
  return m_distance_road;
}

double Route::GetCurvature() const {
  return m_distance_road / m_distance_geo;
}

bool Route::IsEndedEqual() const {
  return m_stops.at(0).stop_name == m_stops.at(m_stops.size() - 1).stop_name;
}

const std::vector<Stop>& Route::GetStops() const {
  return m_stops;
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
  default:
    throw std::runtime_error("Route::BuildGraph");  
  }
}

void Route::SetRoutingSettings(RoutingSettings routing_settings) {
  m_routing_settings = routing_settings;
}

void Route::ComputeDistanceCircleGeo() {
  for (size_t i = 1; i < m_stops.size(); ++i) {
    m_distance_geo += ComputeDistanceBetweenStops(m_stops[i - 1], m_stops[i]);
  }
}

void Route::ComputeDistanceLinearGeo() {
  for (size_t i = 1; i < m_stops.size(); ++i) {
    m_distance_geo += 2 * ComputeDistanceBetweenStops(m_stops[i - 1], m_stops[i]);
  }
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

void Route::ComputeDistanceCircleRoad() {
  m_distance_road = ComputeDistanceRoad(m_stops.cbegin(), m_stops.cend());
}

void Route::ComputeDistanceLinearRoad() {
  // from stop1 to stopN
  m_distance_road = ComputeDistanceRoad(m_stops.cbegin(), m_stops.cend());

  // from stopN to stop1
  m_distance_road += ComputeDistanceRoad(m_stops.crbegin(), m_stops.crend());
}
