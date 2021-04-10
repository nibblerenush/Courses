#include <iostream>
#include <map>
#include <string>
#include <vector>

int main()
{
  const std::string NEW_BUS = "NEW_BUS";
  const std::string BUSES_FOR_STOP = "BUSES_FOR_STOP";
  const std::string STOPS_FOR_BUS = "STOPS_FOR_BUS";
  const std::string ALL_BUSES = "ALL_BUSES";

  std::map<std::string, std::vector<std::string>> bus_stops;
  std::map<std::string, std::vector<std::string>> stop_buses;

  int Q;
  std::cin >> Q;

  for (int counter = 0; counter < Q; ++counter)
  {
    std::string command;
    std::cin >> command;

    // =====
    if (command == NEW_BUS)
    {
      std::string bus;
      std::cin >> bus;

      int stop_count;
      std::cin >> stop_count;

      for (int i = 0; i < stop_count; ++i)
      {
        std::string stop;
        std::cin >> stop;
        bus_stops[bus].push_back(stop);
        stop_buses[stop].push_back(bus);
      }
    }
    // =====
    else if (command == BUSES_FOR_STOP)
    {
      std::string stop;
      std::cin >> stop;

      if (stop_buses.count(stop))
      {
        for (const auto& bus : stop_buses[stop])
        {
          std::cout << bus << ' ';
        }
        std::cout << std::endl;
      }
      else
      {
        std::cout << "No stop" << std::endl;
      }
    }
    // =====
    else if (command == STOPS_FOR_BUS)
    {
      std::string bus_from_cin;
      std::cin >> bus_from_cin;

      if (bus_stops.count(bus_from_cin))
      {
        for (const auto& stop : bus_stops[bus_from_cin])
        {
          std::cout << "Stop " << stop << ": ";
          if (stop_buses[stop].size() == 1)
          {
            std::cout << "no interchange";
          }
          else
          {
            for (const auto& bus : stop_buses[stop])
            {
              if (bus != bus_from_cin)
              {
                std::cout << bus << " ";
              }
            }
          }
          std::cout << std::endl;
        }
      }
      else
      {
        std::cout << "No bus" << std::endl;
      }
    }
    // =====
    else if (command == ALL_BUSES)
    {
      if (bus_stops.size())
      {
        for (const auto& bus_stop : bus_stops)
        {
          std::cout << "Bus " << bus_stop.first << ": ";
          for (const auto& stop : bus_stop.second)
          {
            std::cout << stop << ' ';
          }
          std::cout << std::endl;
        }
      }
      else
      {
        std::cout << "No buses" << std::endl;
      }
    }
  }

  return 0;
}
