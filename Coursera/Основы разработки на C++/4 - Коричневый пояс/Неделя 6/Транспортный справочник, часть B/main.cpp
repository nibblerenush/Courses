#include "profile.h"
#include "test_runner.h"

#include "Bus.h"
#include "GeoCoords.h"
#include "StopsStorage.h"

#include <iostream>
#include <set>
#include <sstream>
#include <string_view>
#include <unordered_set>

using namespace std;

class DatabaseManager {
public:
  void UpdateDatabase(istream& input_stream = cin) {
    int query_count;
    input_stream >> query_count;
    vector<string> postponed_queries;
    
    for (int counter = 0; counter < query_count; counter++) {
      string query_name;
      input_stream >> query_name;

      if (query_name == "Stop") {
        string stop_name;
        GeoCoords stop_geo_coords;

        input_stream.ignore(1);
        getline(input_stream, stop_name, ':');
        input_stream >> stop_geo_coords;

        m_stops_storage.Add(move(stop_name), move(stop_geo_coords));
      }
      else if (query_name == "Bus") {
        string postponed_query;
        input_stream.ignore(1);
        getline(input_stream, postponed_query);
        postponed_queries.push_back(move(postponed_query));
      }
    }

    for (auto& postponed_query : postponed_queries) {
      char delim = postponed_query.find('>') != string::npos ? '>' : '-';
      istringstream postponed_query_iss(move(postponed_query));
      
      string bus_number;
      getline(postponed_query_iss, bus_number, ':');
      Bus bus(bus_number);
      bus.SetRoute(ReadRoute(postponed_query_iss, delim, bus));

      m_buses.emplace(bus.GetNumber(), move(bus));
    }
  }

  void QueryDatabase(istream& input_stream = cin, ostream& output_stream = cout) const {
    int query_count;
    input_stream >> query_count;

    for (int counter = 0; counter < query_count; counter++) {
      string query_name;
      input_stream >> query_name;

      if (query_name == "Bus") {
        input_stream.ignore(1);
        string bus_number;
        getline(input_stream, bus_number);

        output_stream << "Bus " << bus_number << ": ";
        if (m_buses.count(bus_number)) {
          const Bus& bus = m_buses.at(bus_number);
          const Route& route = bus.GetRoute();

          output_stream << route.GetStopsOnRoute() << " stops on route, "
            << route.GetUniqueStops() << " unique stops, "
            << route.GetDistance() << " route length\n";
        }
        else {
          output_stream << "not found\n";
        }
      }
      else if (query_name == "Stop") {
        input_stream.ignore(1);
        string stop_name;
        getline(input_stream, stop_name);
        
        optional<const set<string>*> buses = m_stops_storage.GetBuses(stop_name);

        output_stream << "Stop " << stop_name << ": ";
        if (!buses.has_value()) {
          output_stream << "not found\n";
        }
        else if (buses.value()->empty()) {
          output_stream << "no buses\n";
        }
        else {
          output_stream << "buses";
          for (const auto& bus : *buses.value()) {
            output_stream << ' ' << bus;
          }
          output_stream << '\n';
        }
      }
    }
  }

private:
  Route ReadRoute(istringstream& route_iss, char delim, Bus& bus) {
    Route route;
    
    while (true) {
      route_iss.ignore(1);
      string stop_name;
      if (!getline(route_iss, stop_name, delim)) {
        break;
      }
      if (stop_name.back() == ' ') {
        stop_name.pop_back();
      }
      route.AddStop(stop_name, m_stops_storage);
      m_stops_storage.AddBus(stop_name, bus.GetNumber());
    }

    switch (delim)
    {
    case '>':
      route.SetType(RouteType::CIRCLE);
      break;
    case '-':
      route.SetType(RouteType::LINEAR);
      break;
    }

    return route;
  }

private:
  StopsStorage m_stops_storage;
  unordered_map<string, Bus> m_buses;
};

void CommonTest() {
  stringstream ss_update;
  ss_update << "13\n"
    << "Stop Tolstopaltsevo: 55.611087, 37.20829\n"
    << "Stop Marushkino: 55.595884, 37.209755\n"
    << "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
    << "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
    << "Stop Rasskazovka: 55.632761, 37.333324\n"
    << "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517\n"
    << "Stop Biryusinka: 55.581065, 37.64839\n"
    << "Stop Universam: 55.587655, 37.645687\n"
    << "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n"
    << "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n"
    << "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
    << "Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n"
    << "Stop Prazhskaya: 55.611678, 37.603831\n";

  DatabaseManager database_manager;
  database_manager.UpdateDatabase(ss_update);

  stringstream ss_query;
  ss_query << "6\n"
    << "Bus 256\n"
    << "Bus 750\n"
    << "Bus 751\n"
    << "Stop Samara\n"
    << "Stop Prazhskaya\n"
    << "Stop Biryulyovo Zapadnoye\n";

  database_manager.QueryDatabase(ss_query, cerr);
}

void TestAll() {
  TestRunner tr;
  RUN_TEST(tr, CommonTest);
}

int main() {
  cout.precision(6);
  TestAll();

  DatabaseManager database_manager;
  database_manager.UpdateDatabase();
  database_manager.QueryDatabase();

  return 0;
}
