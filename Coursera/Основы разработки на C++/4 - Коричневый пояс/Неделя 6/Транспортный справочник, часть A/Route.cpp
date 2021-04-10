#include "Route.h"

#include <cmath>
#include <sstream>
#include <unordered_set>
#include <utility>

using namespace std;

void Route::AddStop(string name, const StopsStorage& stops_storage) {
  Stop new_stop;
  new_stop.name = move(name);
  new_stop.geo_coords = stops_storage.GetGeoCoords(new_stop.name);
  m_stops.push_back(move(new_stop));
  m_unique_stops.insert(m_stops.back().name);
}

std::size_t Route::GetStopsOnRoute() const {
  switch (m_type)
  {
  case RouteType::LINEAR:
    return 2 * m_stops.size() - 1;
  case RouteType::CIRCLE:
    return m_stops.size();
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
    ComputeDistanceLinear();
    break;
  case RouteType::CIRCLE:
    ComputeDistanceCircle();
    break;
  }
}

RouteType Route::GetType() const {
  return m_type;
}

double Route::GetDistance() const {
  return m_distance;
}

void Route::ComputeDistanceCircle() {
  m_distance = 0.0;
  for (size_t i = 1; i < m_stops.size(); ++i) {
    m_distance += ComputeDistanceBetweenStops(m_stops[i - 1], m_stops[i]);
  }
}

void Route::ComputeDistanceLinear() {
  m_distance = 0.0;
  for (size_t i = 1; i < m_stops.size(); ++i) {
    m_distance += 2 * ComputeDistanceBetweenStops(m_stops[i - 1], m_stops[i]);
  }
}

double Route::ComputeDistanceBetweenStops(const Stop& lhs, const Stop& rhs) {
  static double pi = 3.1415926535;
  double lhs_lat = lhs.geo_coords.latitude * pi / 180;
  double lhs_lon = lhs.geo_coords.longitude * pi / 180;
  double rhs_lat = rhs.geo_coords.latitude * pi / 180;
  double rhs_lon = rhs.geo_coords.longitude * pi / 180;
  return acos(sin(lhs_lat) * sin(rhs_lat) + cos(lhs_lat) * cos(rhs_lat) * cos(abs(lhs_lon - rhs_lon))) * 6371000;
}
