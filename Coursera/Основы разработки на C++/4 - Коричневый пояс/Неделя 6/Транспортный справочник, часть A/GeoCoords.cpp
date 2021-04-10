#include "GeoCoords.h"

#include <utility>

using namespace std;

istream& operator >> (istream& input_stream, GeoCoords& geo_coords) {
  GeoCoords temp_geo_coords;
  char comma;
  
  if (input_stream >> temp_geo_coords.latitude >> comma >> temp_geo_coords.longitude) {
    geo_coords = move(temp_geo_coords);
  }

  return input_stream;
}
