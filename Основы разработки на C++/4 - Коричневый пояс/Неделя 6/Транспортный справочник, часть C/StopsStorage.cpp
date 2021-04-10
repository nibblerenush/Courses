#include "StopsStorage.h"

#include <utility>

using namespace std;

void StopsStorage::Add(const string& name, GeoCoords geo_coords) {
  m_stops[name].geo_coords = move(geo_coords);
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
