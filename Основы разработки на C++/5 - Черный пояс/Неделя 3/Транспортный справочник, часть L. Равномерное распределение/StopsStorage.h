#pragma once

#include "GeoCoords.h"

#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>

struct StopInfo {
  GeoCoords geo_coords;
  std::set<std::string> buses;
  std::unordered_map<std::string, int> adjacent_stops;
  std::size_t number = 0;
};

class StopsStorage {
public:
  void Add(const std::string& name, GeoCoords geo_coords);
  const StopInfo& GetStopInfo(const std::string& name) const;
  void AddBus(const std::string& name, std::string bus);
  std::optional<const std::set<std::string>*> GetBuses(const std::string& name) const;
  void AddAdjacentStop(const std::string& name, std::string adjacent_stop, int distance);
  std::size_t GetSize() const;
  const std::map<std::string, StopInfo>& GetStops() const;
  void UpdatePaintCoords(const std::string& name, double lon, double lat);

private:
  std::map<std::string, StopInfo> m_stops;
};
