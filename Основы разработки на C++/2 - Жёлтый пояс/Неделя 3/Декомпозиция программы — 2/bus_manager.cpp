#include "bus_manager.h"
#include <sstream>

void BusManager::AddBus(const std::string& bus, const std::vector<std::string>& stops)
{
  m_buses_to_stops[bus] = stops;
  for (const auto& stop : stops)
  {
    m_stops_to_buses[stop].push_back(bus);
  }
}

BusesForStopResponse BusManager::GetBusesForStop(const std::string& stop) const
{
  std::ostringstream osstream;
  if (m_stops_to_buses.count(stop) == 0)
  {
    osstream << "No stop";
  }
  else
  {
    for (const std::string& bus : m_stops_to_buses.at(stop))
    {
      osstream << bus << " ";
    }
  }
  return { osstream.str() };
}

StopsForBusResponse BusManager::GetStopsForBus(const std::string& bus) const
{
  std::ostringstream osstream;
  if (m_buses_to_stops.count(bus) == 0)
  {
    osstream << "No bus";
  }
  else
  {
    for (const std::string& stop : m_buses_to_stops.at(bus))
    {
      osstream << "Stop " << stop << ": ";
      if (m_stops_to_buses.at(stop).size() == 1)
      {
        osstream << "no interchange";
      }
      else
      {
        for (const std::string& other_bus : m_stops_to_buses.at(stop))
        {
          if (bus != other_bus)
          {
            osstream << other_bus << " ";
          }
        }
      }
      osstream << '\n';
    }
  }
  return { osstream.str() };
}

AllBusesResponse BusManager::GetAllBuses() const
{
  std::ostringstream osstream;
  if (m_buses_to_stops.empty())
  {
    osstream << "No buses";
  }
  else
  {
    for (const auto& bus_item : m_buses_to_stops)
    {
      osstream << "Bus " << bus_item.first << ": ";
      for (const std::string& stop : bus_item.second)
      {
        osstream << stop << " ";
      }
      osstream << '\n';
    }
  }
  return { osstream.str() };
}
