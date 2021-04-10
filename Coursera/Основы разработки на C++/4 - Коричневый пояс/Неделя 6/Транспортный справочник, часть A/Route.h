#pragma once

#include "StopsStorage.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Stop {
  std::string name;
  GeoCoords geo_coords;
};

enum class RouteType {
  LINEAR,
  CIRCLE,
  UNKNOWN
};

class Route {
public:
  void AddStop(std::string name, const StopsStorage& stops_storage);
  void SetType(RouteType type);
  RouteType GetType() const;
  std::size_t GetStopsOnRoute() const;
  std::size_t GetUniqueStops() const;
  double GetDistance() const;

private:
  void ComputeDistanceCircle();
  void ComputeDistanceLinear();
  static double ComputeDistanceBetweenStops(const Stop& lhs, const Stop& rhs);

private:
  std::vector<Stop> m_stops;
  std::unordered_set<std::string> m_unique_stops;
  RouteType m_type = RouteType::UNKNOWN;
  double m_distance = 0.0;
};
