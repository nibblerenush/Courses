#include "StopsStorage.h"

#include <utility>

using namespace std;

void StopsStorage::Add(string name, GeoCoords geo_coords) {
  m_stops.emplace(move(name), move(geo_coords));
}

GeoCoords StopsStorage::GetGeoCoords(const string& name) const {
  return m_stops.at(name);
}
