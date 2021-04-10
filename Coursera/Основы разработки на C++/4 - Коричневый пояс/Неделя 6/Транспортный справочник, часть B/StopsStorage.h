#pragma once

#include "GeoCoords.h"

#include <optional>
#include <set>
#include <string>
#include <unordered_map>

struct StopInfo {
  GeoCoords geo_coords;
  std::set<std::string> buses;
};

class StopsStorage {
public:
  void Add(std::string name, GeoCoords geo_coords);
  GeoCoords GetGeoCoords(const std::string& name) const;
  void AddBus(const std::string& name, std::string bus);
  std::optional<const std::set<std::string>*> GetBuses(const std::string& name) const;
private:
  std::unordered_map<std::string, StopInfo> m_stops;
};
