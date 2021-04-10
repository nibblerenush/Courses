#include "query.h"

std::istream& operator >> (std::istream& is, Query& q)
{
  std::string command;
  is >> command;

  if (command == "NEW_BUS")
  {
    q.type = QueryType::NewBus;
    is >> q.bus;
    int stop_count;
    is >> stop_count;
    q.stops.resize(stop_count);
    for (auto& stop : q.stops) { is >> stop; }
  }
  else if (command == "BUSES_FOR_STOP")
  {
    q.type = QueryType::BusesForStop;
    is >> q.stop;
  }
  else if (command == "STOPS_FOR_BUS")
  {
    q.type = QueryType::StopsForBus;
    is >> q.bus;
  }
  else if (command == "ALL_BUSES")
  {
    q.type = QueryType::AllBuses;
  }

  return is;
}
