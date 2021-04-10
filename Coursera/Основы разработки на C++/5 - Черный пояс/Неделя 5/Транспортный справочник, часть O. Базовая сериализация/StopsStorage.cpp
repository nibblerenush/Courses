#include "StopsStorage.h"

#include <utility>

using namespace std;

void StopsStorage::Add(const string& name, GeoCoords geo_coords) {
  m_stops[name].geo_coords = move(geo_coords);
  m_stops[name].geo_coords.paint_lat = m_stops[name].geo_coords.latitude;
  m_stops[name].geo_coords.paint_lon = m_stops[name].geo_coords.longitude;
  m_stops[name].number = m_stops.size() - 1;
}

void StopsStorage::AddBus(const std::string& name, std::string bus) {
  m_stops.at(name).buses.insert(move(bus));
}

void StopsStorage::AddAdjacentStop(const string& name, string adjacent_stop, int distance) {
  m_stops[name].adjacent_stops[move(adjacent_stop)] = distance;
}

const StopInfo& StopsStorage::GetStopInfo(const string& name) const {
  return m_stops.at(name);
}

optional<const set<string>*> StopsStorage::GetBuses(const std::string& name) const {
  if (!m_stops.count(name)) {
    return nullopt;
  }
  return &m_stops.at(name).buses;
}

const std::map<std::string, StopInfo>& StopsStorage::GetStops() const {
  return m_stops;
}

std::size_t StopsStorage::GetSize() const {
  return m_stops.size();
}

void StopsStorage::UpdatePaintCoords(const std::string& name, double lon, double lat) {
  m_stops.at(name).geo_coords.paint_lon = lon;
  m_stops.at(name).geo_coords.paint_lat = lat;
}
