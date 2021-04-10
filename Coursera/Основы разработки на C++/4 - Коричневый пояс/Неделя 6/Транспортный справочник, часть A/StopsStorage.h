#pragma once

#include "GeoCoords.h"

#include <string>
#include <unordered_map>

class StopsStorage {
public:
  void Add(std::string name, GeoCoords geo_coords);
  GeoCoords GetGeoCoords(const std::string& name) const;
private:
  std::unordered_map<std::string, GeoCoords> m_stops;
};
