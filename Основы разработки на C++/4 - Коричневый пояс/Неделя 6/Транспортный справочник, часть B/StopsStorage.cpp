#include "StopsStorage.h"

#include <utility>

using namespace std;

void StopsStorage::Add(string name, GeoCoords geo_coords) {
  m_stops.emplace(move(name), StopInfo{move(geo_coords), set<string>()});
}

GeoCoords StopsStorage::GetGeoCoords(const string& name) const {
  return m_stops.at(name).geo_coords;
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
