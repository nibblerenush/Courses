#pragma once

#include "graph.h"
#include "StopsStorage.h"

#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

struct GraphWeight {
  GraphWeight() = default;
  GraphWeight(int wt, double bt);
  GraphWeight(int zero);
  
  int wait_time = 0;
  double bus_time = 0.0;
  std::string_view bus_name;
  std::string_view from_stop_name;
  int count = 0;
  
  std::size_t from_idx = 0;
  std::size_t to_idx = 0;
};

// for Graph operations
GraphWeight operator + (const GraphWeight& lhs, const GraphWeight& rhs);
bool operator < (const GraphWeight& lhs, const GraphWeight& rhs);
bool operator > (const GraphWeight& lhs, const GraphWeight& rhs);
bool operator >= (const GraphWeight& lhs, int rhs);

struct Stop {
  const std::string& stop_name;
  const StopInfo& stop_info;
};

struct RoutingSettings {
  int bus_wait_time = 0;
  double bus_velocity = 0.0;
};

enum class RouteType {
  LINEAR,
  CIRCLE
};

class Route {
public:
  // Non const
  Route(const std::string& route_name);
  void AddStop(const std::string& stop_name, const StopsStorage& stops_storage);
  void SetType(RouteType type);
  void CommonSetType(RouteType type);

  // Const
  RouteType GetType() const;
  const std::string& GetName() const;
  std::size_t GetStopsOnRoute() const;
  std::size_t GetUniqueStops() const;
  int GetDistance() const;
  double GetCurvature() const;
  bool IsEndedEqual() const;
  const std::vector<Stop>& GetStops() const;
  void BuildGraph(Graph::DirectedWeightedGraph<GraphWeight>& graph) const;

  // Static
  static void SetRoutingSettings(RoutingSettings routing_settings);

private:
  void ComputeDistanceCircleGeo();
  void ComputeDistanceLinearGeo();
  static double ComputeDistanceBetweenStops(const Stop& lhs, const Stop& rhs);

  void ComputeDistanceCircleRoad();
  void ComputeDistanceLinearRoad();

  template <typename InputIt>
  int ComputeDistanceRoad(InputIt start, InputIt end) const {
    int distance = 0;
    for (auto iter = start; iter != std::prev(end); ++iter) {
      std::string_view next_stop = std::next(iter)->stop_name;
      if (iter->stop_info.adjacent_stops.count(next_stop)) {
        distance += iter->stop_info.adjacent_stops.at(next_stop);
      }
      else {
        std::string_view current_stop = iter->stop_name;
        distance += std::next(iter)->stop_info.adjacent_stops.at(current_stop);
      }
    }
    return distance;
  }
  
  template <typename InputIt>
  void ComputeGraph(InputIt start, InputIt end, Graph::DirectedWeightedGraph<GraphWeight>& graph) const {
    for (InputIt iter = start; iter != end; ++iter) {
      // from iter to std::next(subiter)
      int count = 0;
      int distance = 0;
      
      for (InputIt subiter = iter; subiter != std::prev(end); ++subiter) {
        // Not work for same stops
        if (iter->stop_info.number == std::next(subiter)->stop_info.number) {
          continue;
        }

        std::string_view next_stop = std::next(subiter)->stop_name;
        if (subiter->stop_info.adjacent_stops.count(next_stop)) {
          distance += subiter->stop_info.adjacent_stops.at(next_stop);
        }
        else {
          std::string_view current_stop = subiter->stop_name;
          distance += std::next(subiter)->stop_info.adjacent_stops.at(current_stop);
        }
        count++;
        
        GraphWeight graph_node;
        graph_node.wait_time = m_routing_settings.bus_wait_time;
        graph_node.bus_time = distance / m_routing_settings.bus_velocity;
        graph_node.bus_name = m_route_name;
        graph_node.from_stop_name = iter->stop_name;
        graph_node.count = count;
        
        if constexpr (std::is_same<InputIt, std::vector<Stop>::const_iterator>::value) {
          graph_node.from_idx = std::distance(m_stops.cbegin(), iter);
          graph_node.to_idx = std::distance(m_stops.cbegin(), std::next(subiter));
        }
        else if constexpr (std::is_same<InputIt, std::vector<Stop>::const_reverse_iterator>::value) {
          graph_node.from_idx = std::distance(m_stops.cbegin(), iter.base() - 1);
          graph_node.to_idx = std::distance(m_stops.cbegin(), std::next(subiter).base() - 1);
        }

        graph.AddEdge(Graph::Edge<GraphWeight>{
          iter->stop_info.number,
          std::next(subiter)->stop_info.number,
          std::move(graph_node)});
      }
    }
  }

private:
  std::string m_route_name;
  std::vector<Stop> m_stops;
  std::unordered_set<std::string_view> m_unique_stops;
  RouteType m_type;

  double m_distance_geo = 0.0;
  int m_distance_road = 0;

  static RoutingSettings m_routing_settings;
};
