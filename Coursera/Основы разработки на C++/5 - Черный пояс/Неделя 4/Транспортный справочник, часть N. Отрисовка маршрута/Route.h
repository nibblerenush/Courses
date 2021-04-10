#pragma once

#include "graph.h"
#include "StopsStorage.h"

#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

struct Stop {
  std::string name;
  StopInfo stop_info;
};

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
  int count = 0;

  std::vector<Stop>::const_iterator from_iter;
  std::vector<Stop>::const_iterator to_iter;
};

GraphWeight operator+(const GraphWeight& lhs, const GraphWeight& rhs);
bool operator<(const GraphWeight& lhs, const GraphWeight& rhs);
bool operator>(const GraphWeight& lhs, const GraphWeight& rhs);
bool operator>=(const GraphWeight& lhs, int rhs);

struct RoutingSettings {
  int bus_wait_time = 0;
  double bus_velocity = 0.0;
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
  const std::vector<Stop>& GetStops() const;
  bool IsEndedEqual() const;
  void UpdatePaintCoords(const StopsStorage& stops_storage);

private:
  void ComputeDistanceCircleGeo();
  void ComputeDistanceLinearGeo();

  void ComputeDistanceCircleRoad();
  void ComputeDistanceLinearRoad();

  template <typename InputIt>
  int ComputeDistanceRoad(InputIt start, InputIt end)
  {
    int distance = 0;
    for (auto iter = start; iter != std::prev(end); ++iter) {
      const std::string& next_stop = std::next(iter)->name;
      if (iter->stop_info.adjacent_stops.count(next_stop)) {
        distance += iter->stop_info.adjacent_stops[next_stop];
      }
      else {
        const std::string& current_stop = iter->name;
        distance += std::next(iter)->stop_info.adjacent_stops[current_stop];
      }
    }
    return distance;
  }

  template <typename InputIt>
  void ComputeGraph(InputIt start, InputIt end, Graph::DirectedWeightedGraph<GraphWeight>& graph) const
  {
    for (InputIt iter = start; iter != end; ++iter)
    {
      GraphWeight start_graph_weight(0, 0.0);
      start_graph_weight.bus_name = m_bus_name;
      start_graph_weight.from_stop_name = iter->name;
      start_graph_weight.count = 0;
      
      if constexpr (std::is_same<InputIt, std::vector<Stop>::const_iterator>::value) {
        start_graph_weight.from_iter = iter;
        start_graph_weight.to_iter = iter;
      }
      else if constexpr (std::is_same<InputIt, std::vector<Stop>::const_reverse_iterator>::value) {
        start_graph_weight.from_iter = iter.base() - 1;
        start_graph_weight.to_iter = iter.base() - 1;
      }

      Graph::Edge<GraphWeight> edge{ iter->stop_info.number, iter->stop_info.number, std::move(start_graph_weight) };
      graph.AddEdge(std::move(edge));
      
      int distance = 0;
      int count = 0;
      for (InputIt subiter = iter; subiter != std::prev(end) && subiter != end; ++subiter)
      {
        const std::string& next_stop = std::next(subiter)->name;
        if (subiter->stop_info.adjacent_stops.count(next_stop)) {
          distance += subiter->stop_info.adjacent_stops.at(next_stop);
        }
        else {
          const std::string& current_stop = subiter->name;
          distance += std::next(subiter)->stop_info.adjacent_stops.at(current_stop);
        }
        count++;
        
        GraphWeight graph_weight(m_routing_settings.bus_wait_time, distance / m_routing_settings.bus_velocity);
        graph_weight.bus_name = m_bus_name;
        graph_weight.from_stop_name = iter->name;
        graph_weight.count = count;

        if constexpr (std::is_same<InputIt, std::vector<Stop>::const_iterator>::value) {
          graph_weight.from_iter = iter;
          graph_weight.to_iter = std::next(subiter);
        }
        else if constexpr (std::is_same<InputIt, std::vector<Stop>::const_reverse_iterator>::value) {
          graph_weight.from_iter = iter.base() - 1;
          graph_weight.to_iter = std::next(subiter).base() - 1;
        }
        
        Graph::Edge<GraphWeight> edge{ iter->stop_info.number, std::next(subiter)->stop_info.number, std::move(graph_weight) };
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
