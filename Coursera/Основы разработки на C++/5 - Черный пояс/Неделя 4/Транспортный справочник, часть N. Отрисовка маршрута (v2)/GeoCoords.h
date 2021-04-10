#pragma once

struct GeoCoords {
  GeoCoords() = default;
  GeoCoords(double init_lat, double init_lon);

  double latitude = 0.0;
  double longitude = 0.0;

  double paint_lat = 0.0;
  double paint_lon = 0.0;
};
