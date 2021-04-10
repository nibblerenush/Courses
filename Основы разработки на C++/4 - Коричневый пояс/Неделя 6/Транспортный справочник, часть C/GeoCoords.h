#pragma once

#include <iostream>

struct GeoCoords {
  double latitude = 0.0;
  double longitude = 0.0;
};

std::istream& operator >> (std::istream& input_stream, GeoCoords& geo_coords);
