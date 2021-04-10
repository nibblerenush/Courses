#pragma once

#include <map>
#include <string>
#include <vector>

#include "responses.h"

class BusManager
{
public:
  void AddBus(const std::string& bus, const std::vector<std::string>& stops);
  BusesForStopResponse GetBusesForStop(const std::string& stop) const;
  StopsForBusResponse GetStopsForBus(const std::string& bus) const;
  AllBusesResponse GetAllBuses() const;

private:
  std::map<std::string, std::vector<std::string>> m_buses_to_stops;
  std::map<std::string, std::vector<std::string>> m_stops_to_buses;
};
