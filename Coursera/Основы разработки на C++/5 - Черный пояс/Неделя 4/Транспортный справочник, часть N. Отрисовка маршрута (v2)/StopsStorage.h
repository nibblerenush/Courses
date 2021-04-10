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
  std::set<std::string_view> bus_names;
  std::unordered_map<std::string_view, int> adjacent_stops;
  std::size_t number = 0;
};

class StopsStorage {
public:
  // Non const
  void Add(const std::string& stop_name);
  void Add(const std::string& stop_name, const GeoCoords& geo_coords);
  void AddBus(const std::string& stop_name, std::string_view bus_name);
  void AddAdjacentStop(const std::string& stop_name, const std::string& adjacent_stop, int distance);
  void UpdatePaintCoords(const std::string& stop_name, double lon, double lat);

  std::optional<const std::set<std::string_view>*> GetBuses(const std::string& stop_name) const;
  const std::map<std::string, StopInfo>& GetStops() const;
  
private:
  void SetNumber(const std::string& stop_name);

private:
  std::map<std::string, StopInfo> m_stops;
  std::size_t m_number = 0;
};
