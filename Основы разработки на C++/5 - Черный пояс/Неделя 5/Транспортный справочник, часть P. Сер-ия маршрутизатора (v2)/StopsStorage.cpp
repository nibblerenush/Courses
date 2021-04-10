#include "StopsStorage.h"

#include <stdexcept>
#include <utility>

void StopsStorage::Add(const std::string& stop_name) {
  if (!m_stops.count(stop_name)) {
    m_stops.emplace(stop_name, StopInfo{});
  }
}

void StopsStorage::Add(const std::string& stop_name, const GeoCoords& geo_coords) {
  m_stops[stop_name].geo_coords = geo_coords;
  SetNumber(stop_name);
}

void StopsStorage::AddBus(const std::string& stop_name, std::string_view bus_name) {
  m_stops[stop_name].bus_names.insert(bus_name);
}

void StopsStorage::AddAdjacentStop(const std::string& stop_name, const std::string& adjacent_stop, int distance) {
  auto iter = m_stops.find(adjacent_stop);
  if (iter != m_stops.end()) {
    m_stops[stop_name].adjacent_stops[iter->first] = distance;
  }
  else {
    throw std::runtime_error("StopsStorage::AddAdjacentStop");
  }
}

void StopsStorage::UpdatePaintCoords(const std::string& stop_name, double lon, double lat) {
  m_stops[stop_name].geo_coords.paint_lon = lon;
  m_stops[stop_name].geo_coords.paint_lat = lat;
}

std::optional<const std::set<std::string_view>*> StopsStorage::GetBuses(const std::string& stop_name) const {
  if (!m_stops.count(stop_name)) {
    return std::nullopt;
  }
  return &m_stops.at(stop_name).bus_names;
}

const std::map<std::string, StopInfo>& StopsStorage::GetStops() const {
  return m_stops;
}

void StopsStorage::SetNumber(const std::string& stop_name) {
  m_stops[stop_name].number = m_number++;
}
