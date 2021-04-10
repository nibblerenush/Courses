#pragma once

#include "GeoCoords.h"

#include <map>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>

struct StopInfo {
  GeoCoords geo_coords;
  std::set<std::string> buses;
  std::unordered_map<std::string, int> adjacent_stops;
  std::size_t number = 0;
};

class StopsStorage {
public:
  // non const
  void Add(const std::string& name, GeoCoords geo_coords);
  void AddBus(const std::string& name, std::string bus);
  void AddAdjacentStop(const std::string& name, std::string adjacent_stop, int distance);

  // const
  const StopInfo& GetStopInfo(const std::string& name) const;
  std::optional<const std::set<std::string>*> GetBuses(const std::string& name) const;
  const std::map<std::string, StopInfo>& GetStops() const;
  std::size_t GetSize() const;

  // for paint
  void UpdatePaintCoords(const std::string& name, double lon, double lat);

private:
  std::map<std::string, StopInfo> m_stops;
};
