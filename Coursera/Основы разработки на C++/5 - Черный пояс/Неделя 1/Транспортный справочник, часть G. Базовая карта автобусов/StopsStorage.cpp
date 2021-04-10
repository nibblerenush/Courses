#include "StopsStorage.h"

#include <utility>

using namespace std;

void StopsStorage::Add(const string& name, GeoCoords geo_coords)
{
  m_stops[name].geo_coords = move(geo_coords);
  m_stops[name].number = m_stops.size() - 1;

  if (!min_lat.has_value()) {
    min_lat = geo_coords.latitude;
  }
  else if (geo_coords.latitude < min_lat) {
    min_lat = geo_coords.latitude;
  }

  if (!max_lat.has_value()) {
    max_lat = geo_coords.latitude;
  }
  else if (geo_coords.latitude > max_lat) {
    max_lat = geo_coords.latitude;
  }

  if (!min_lon.has_value()) {
    min_lon = geo_coords.longitude;
  }
  else if (geo_coords.longitude < min_lon) {
    min_lon = geo_coords.longitude;
  }

  if (!max_lon.has_value()) {
    max_lon = geo_coords.longitude;
  }
  else if (geo_coords.longitude > max_lon) {
    max_lon = geo_coords.longitude;
  }
}

const StopInfo& StopsStorage::GetStopInfo(const string& name) const {
  return m_stops.at(name);
}

void StopsStorage::AddBus(const std::string& name, std::string bus) {
  m_stops.at(name).buses.insert(move(bus));
}

optional<const set<string>*> StopsStorage::GetBuses(const std::string& name) const {
  if (!m_stops.count(name)) {
    return nullopt;
  }
  return &m_stops.at(name).buses;
}

void StopsStorage::AddAdjacentStop(const string& name, string adjacent_stop, int distance) {
  m_stops[name].adjacent_stops[move(adjacent_stop)] = distance;
}

std::size_t StopsStorage::GetSize() const {
  return m_stops.size();
}

const std::map<std::string, StopInfo>& StopsStorage::GetStops() const {
  return m_stops;
}
