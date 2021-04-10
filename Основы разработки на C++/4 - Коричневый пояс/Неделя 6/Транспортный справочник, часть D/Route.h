#pragma once

#include "StopsStorage.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Stop {
  std::string name;
  StopInfo stop_info;
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
  int GetDistance() const;
  double GetCurvature() const;

private:
  void ComputeDistanceCircleGeo();
  void ComputeDistanceLinearGeo();

  void ComputeDistanceCircleRoad();
  void ComputeDistanceLinearRoad();

  template <typename InputIt>
  int ComputeDistanceRoad(InputIt start, InputIt end) {
    int distance = 0;
    for (auto iter = start; iter != prev(end); ++iter) {
      const std::string& next_stop = next(iter)->name;
      if (iter->stop_info.adjacent_stops.count(next_stop)) {
        distance += iter->stop_info.adjacent_stops[next_stop];
      }
      else {
        const std::string& current_stop = iter->name;
        distance += next(iter)->stop_info.adjacent_stops[current_stop];
      }
    }
    return distance;
  }

  static double ComputeDistanceBetweenStops(const Stop& lhs, const Stop& rhs);

private:
  std::vector<Stop> m_stops;
  std::unordered_set<std::string> m_unique_stops;
  RouteType m_type = RouteType::UNKNOWN;
  double m_distance_geo = 0.0;
  int m_distance_road = 0;
};
