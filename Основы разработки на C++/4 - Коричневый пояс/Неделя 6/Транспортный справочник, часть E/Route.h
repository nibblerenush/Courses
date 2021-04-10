#pragma once

#include "graph.h"
#include "StopsStorage.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct GraphWeight {
  GraphWeight(int wt, double bt) : wait_time(wt), bus_time(bt)
  {}
  
  GraphWeight(int zero)
  {
    if (zero != 0) {
      throw std::runtime_error("zero != 0");
    }

    wait_time = 0;
    bus_time = 0.0;
  }

  int wait_time = 0;
  double bus_time = 0.0;

  std::string bus_name;
  std::string from_stop_name;
  int count;
};

GraphWeight operator+(const GraphWeight& lhs, const GraphWeight& rhs);
bool operator<(const GraphWeight& lhs, const GraphWeight& rhs);
bool operator>(const GraphWeight& lhs, const GraphWeight& rhs);
bool operator>=(const GraphWeight& lhs, int rhs);

struct RoutingSettings {
  int bus_wait_time = 0;
  double bus_velocity = 0.0;
};

struct Stop {
  std::string name;
  StopInfo stop_info;
};

enum class RouteType {
  LINEAR,
  CIRCLE,
  UNKNOWN
};

class Route {
public:
  void AddStop(std::string name, const StopsStorage& stops_storage);
  void SetType(RouteType type);
  RouteType GetType() const;
  std::size_t GetStopsOnRoute() const;
  std::size_t GetUniqueStops() const;
  int GetDistance() const;
  double GetCurvature() const;
  void SetRoutingSettings(RoutingSettings routing_settings);
  void SetBusName(const std::string& bus_name);
  void BuildGraph(Graph::DirectedWeightedGraph<GraphWeight>& graph) const;

private:
  void ComputeDistanceCircleGeo();
  void ComputeDistanceLinearGeo();

  void ComputeDistanceCircleRoad();
  void ComputeDistanceLinearRoad();

  template <typename InputIt>
  int ComputeDistanceRoad(InputIt start, InputIt end) {
    int distance = 0;
    for (auto iter = start; iter != prev(end); ++iter) {
      const std::string& next_stop = next(iter)->name;
      if (iter->stop_info.adjacent_stops.count(next_stop)) {
        distance += iter->stop_info.adjacent_stops[next_stop];
      }
      else {
        const std::string& current_stop = iter->name;
        distance += next(iter)->stop_info.adjacent_stops[current_stop];
      }
    }
    return distance;
  }

  template <typename InputIt>
  void ComputeGraph(InputIt start, InputIt end, Graph::DirectedWeightedGraph<GraphWeight>& graph) const
  {
    for (auto iter = start; iter != end; ++iter)
    {
      GraphWeight graph_weight(0, 0.0);
      graph_weight.bus_name = m_bus_name;
      graph_weight.from_stop_name = iter->name;
      graph_weight.count = 0;
      Graph::Edge<GraphWeight> edge{ iter->stop_info.number, iter->stop_info.number, std::move(graph_weight) };
      graph.AddEdge(std::move(edge));
      
      int distance = 0;
      int count = 0;
      for (auto subiter = iter; subiter != prev(end) && subiter != end; ++subiter)
      {
        const std::string& next_stop = next(subiter)->name;
        if (subiter->stop_info.adjacent_stops.count(next_stop)) {
          distance += subiter->stop_info.adjacent_stops.at(next_stop);
        }
        else {
          const std::string& current_stop = subiter->name;
          distance += next(subiter)->stop_info.adjacent_stops.at(current_stop);
        }
        count++;
        
        GraphWeight graph_weight(m_routing_settings.bus_wait_time, distance / m_routing_settings.bus_velocity);
        graph_weight.bus_name = m_bus_name;
        graph_weight.from_stop_name = iter->name;
        graph_weight.count = count;
        Graph::Edge<GraphWeight> edge{ iter->stop_info.number, next(subiter)->stop_info.number, std::move(graph_weight) };
        graph.AddEdge(std::move(edge));
      }
    }
  }

  static double ComputeDistanceBetweenStops(const Stop& lhs, const Stop& rhs);

private:
  std::vector<Stop> m_stops;
  std::unordered_set<std::string> m_unique_stops;
  RouteType m_type = RouteType::UNKNOWN;
  double m_distance_geo = 0.0;
  int m_distance_road = 0;
  RoutingSettings m_routing_settings;
  std::string m_bus_name;
};
