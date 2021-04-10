#include "json.h"
#include "router.h"

#include "Paint.h"
#include "Route.h"
#include "StopsStorage.h"

#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>

using namespace std;

string Space(int number) {
  return string(number, ' ');
}

class DatabaseManager {
public:
  void ReadRoutingSettings(const Json::Node& root_node) {
    const Json::Node& json_routing_settings = root_node.AsMap().at("routing_settings");
    RoutingSettings routing_settings;
    routing_settings.bus_wait_time = static_cast<int>(json_routing_settings.AsMap().at("bus_wait_time").AsNumber());
    routing_settings.bus_velocity = json_routing_settings.AsMap().at("bus_velocity").AsNumber() * 1000.0 / 60.0;
    Route::SetRoutingSettings(routing_settings);
  }
  
  void ReadRenderSettings(const Json::Node& root_node) {
    const Json::Node& render_settings = root_node.AsMap().at("render_settings");
    m_render_settings.width = render_settings.AsMap().at("width").AsNumber();
    m_render_settings.height = render_settings.AsMap().at("height").AsNumber();
    m_render_settings.padding = render_settings.AsMap().at("padding").AsNumber();
    m_render_settings.stop_radius = render_settings.AsMap().at("stop_radius").AsNumber();
    m_render_settings.line_width = render_settings.AsMap().at("line_width").AsNumber();
    m_render_settings.stop_label_font_size = static_cast<int>(render_settings.AsMap().at("stop_label_font_size").AsNumber());
    m_render_settings.stop_label_offset.x = render_settings.AsMap().at("stop_label_offset").AsArray().at(0).AsNumber();
    m_render_settings.stop_label_offset.y = render_settings.AsMap().at("stop_label_offset").AsArray().at(1).AsNumber();
    
    auto get_color = [](const Json::Node& node) -> Svg::Color {
      try {
        return node.AsString();
      }
      catch (std::bad_variant_access&) {
        std::size_t color_size = node.AsArray().size();
        const auto& color_array = node.AsArray();
        if (color_size == 3) {
          return Svg::Rgb(
            static_cast<std::uint8_t>(color_array.at(0).AsNumber()),
            static_cast<std::uint8_t>(color_array.at(1).AsNumber()),
            static_cast<std::uint8_t>(color_array.at(2).AsNumber()));
        }
        else {
          return Svg::Rgba(
            static_cast<std::uint8_t>(color_array.at(0).AsNumber()),
            static_cast<std::uint8_t>(color_array.at(1).AsNumber()),
            static_cast<std::uint8_t>(color_array.at(2).AsNumber()),
            color_array.at(3).AsNumber());
        }
      }
    };
    
    m_render_settings.underlayer_color = get_color(render_settings.AsMap().at("underlayer_color"));
    m_render_settings.underlayer_width = render_settings.AsMap().at("underlayer_width").AsNumber();
    for (const Json::Node& color_node : render_settings.AsMap().at("color_palette").AsArray()) {
      m_render_settings.color_palette.push_back(get_color(color_node));
    }

    m_render_settings.bus_label_font_size = static_cast<int>(render_settings.AsMap().at("bus_label_font_size").AsNumber());
    m_render_settings.bus_label_offset.x = render_settings.AsMap().at("bus_label_offset").AsArray().at(0).AsNumber();
    m_render_settings.bus_label_offset.y = render_settings.AsMap().at("bus_label_offset").AsArray().at(1).AsNumber();

    for (const Json::Node& layer : render_settings.AsMap().at("layers").AsArray()) {
      m_render_settings.layers.push_back(layer.AsString());
    }

    m_render_settings.outer_margin = render_settings.AsMap().at("outer_margin").AsNumber();
  }

  void UpdateDatabase(const Json::Node& root_node) {
    const Json::Node& base_requests = root_node.AsMap().at("base_requests");
    
    vector<const Json::Node*> postponed_requests;
    for (const Json::Node& base_request : base_requests.AsArray()) {
      // Get query name
      const string& query_name = base_request.AsMap().at("type").AsString();

      if (query_name == "Stop") {
        BaseRequestStopHandle(base_request);
      }
      else if (query_name == "Bus") {
        postponed_requests.push_back(&base_request);
      }
    }

    for (const Json::Node* bus_node : postponed_requests) {
      BaseRequestBusHandle(bus_node);
    }

    BuildGraph();
    BuildPaint();
  }

  void QueryDatabase(const Json::Node& root_node, ostream& output_stream = cout) const {
    const Json::Node& stat_requests = root_node.AsMap().at("stat_requests");

    output_stream << "[\n";
    bool first = true;
    for (const Json::Node& stat_request : stat_requests.AsArray()) {
      // Get query name
      const string& query_name = stat_request.AsMap().at("type").AsString();

      if (first) {
        first = false;
      }
      else {
        output_stream << ",\n";
      }

      output_stream << Space(2) << "{\n";
      if (query_name == "Bus") {
        StatRequestBusHandle(stat_request, output_stream);
      }
      else if (query_name == "Stop") {
        StatRequestStopHandle(stat_request, output_stream);
      }
      else if (query_name == "Route") {
        StatRequestRouteHandle(stat_request, output_stream);
      }
      else if (query_name == "Map") {
        StatRequestMapHandle(stat_request, output_stream);
      }
      output_stream << Space(2) << "}";
    }
    output_stream << "\n]\n";
  }

private:
  void BaseRequestStopHandle(const Json::Node& stop_node) {
    // Add geo coords
    const string& stop_name = stop_node.AsMap().at("name").AsString();
    GeoCoords stop_geo_coords = {
      stop_node.AsMap().at("latitude").AsNumber(),
      stop_node.AsMap().at("longitude").AsNumber()
    };
    m_stops_storage.Add(stop_name, stop_geo_coords);

    // Add adjacent stops
    if (stop_node.AsMap().count("road_distances")) {
      for (const auto& road_distance : stop_node.AsMap().at("road_distances").AsMap()) {
        m_stops_storage.Add(road_distance.first);
        m_stops_storage.AddAdjacentStop(stop_name, road_distance.first, static_cast<int>(road_distance.second.AsNumber()));
      }
    }
  }

  void BaseRequestBusHandle(const Json::Node* bus_node) {
    const std::string& bus_name = bus_node->AsMap().at("name").AsString();
    m_routes.push_back(Route(bus_name));
    Route& route = m_routes.back();

    for (const Json::Node& stop : bus_node->AsMap().at("stops").AsArray()) {
      const string& stop_name = stop.AsString();
      route.AddStop(stop_name, m_stops_storage);
      m_stops_storage.AddBus(stop_name, route.GetName());
    }
    
    bool is_roundtrip = bus_node->AsMap().at("is_roundtrip").AsBool();
    if (is_roundtrip) {
      route.SetType(RouteType::CIRCLE);
    }
    else {
      route.SetType(RouteType::LINEAR);
    }

    m_buses.emplace(route.GetName(), route);
  }

  void StatRequestBusHandle(const Json::Node& bus_node, ostream& output_stream = cout) const {
    const string& bus_number = bus_node.AsMap().at("name").AsString();
    int64_t id = static_cast<int64_t>(bus_node.AsMap().at("id").AsNumber());

    if (m_buses.count(bus_number)) {
      const Route& route = m_buses.at(bus_number);
      output_stream
        << Space(4) << "\"route_length\": " << route.GetDistance() << ",\n"
        << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"curvature\": " << route.GetCurvature() << ",\n"
        << Space(4) << "\"stop_count\": " << route.GetStopsOnRoute() << ",\n"
        << Space(4) << "\"unique_stop_count\": " << route.GetUniqueStops() << '\n';
    }
    else {
      output_stream
        << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"error_message\": " << "\"not found\"\n";
    }
  }

  void StatRequestStopHandle(const Json::Node& stop_node, ostream& output_stream = cout) const {
    const string& stop_name = stop_node.AsMap().at("name").AsString();
    int64_t id = static_cast<int64_t>(stop_node.AsMap().at("id").AsNumber());

    optional<const set<string_view>*> buses = m_stops_storage.GetBuses(stop_name);
    if (!buses.has_value()) {
      output_stream
        << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"error_message\": " << "\"not found\"\n";
    }
    else if (buses.value()->empty()) {
      output_stream
        << Space(4) << "\"buses\": [],\n"
        << Space(4) << "\"request_id\": " << id << '\n';
    }
    else {
      output_stream << Space(4) << "\"buses\": [\n";
      bool first = true;
      for (const auto& bus : *buses.value()) {
        if (first) {
          first = false;
        }
        else {
          output_stream << ",\n";
        }
        output_stream << Space(6) << '\"' << bus << '\"';
      }
      output_stream
        << '\n' << Space(4) << "],\n"
        << Space(4) << "\"request_id\": " << id << '\n';
    }
  }
  
  void StatRequestRouteHandle(const Json::Node& route_node, ostream& output_stream = cout) const {
    int64_t id = static_cast<int64_t>(route_node.AsMap().at("id").AsNumber());
    const string& from = route_node.AsMap().at("from").AsString();
    const string& to = route_node.AsMap().at("to").AsString();
    auto result = m_router->BuildRoute(m_stops_storage.GetStops().at(from).number, m_stops_storage.GetStops().at(to).number);
    
    // Help for paint
    std::vector<BusElement> bus_elems;
    std::vector<WaitElement> wait_elems;
    
    if (result.has_value()) {
      if (result.value().edge_count != 0) {
        output_stream << Space(4) << "\"items\": [\n";
        for (std::size_t i = 0; i < result.value().edge_count; ++i) {
          Graph::EdgeId edgeId = m_router->GetRouteEdge(result.value().id, i);
          const Graph::Edge<GraphWeight>& edge = m_graph->GetEdge(edgeId);

          if (edge.weight.wait_time != 0) {
            output_stream
              << Space(6) << "{\n"
              << Space(8) << "\"time\": " << edge.weight.wait_time << ",\n"
              << Space(8) << "\"stop_name\": " << '\"' << edge.weight.from_stop_name << "\",\n"
              << Space(8) << "\"type\": " << "\"Wait\"\n"
              << Space(6) << "},\n";
            wait_elems.push_back(WaitElement{edge.weight.from_stop_name});
          }

          output_stream
            << Space(6) << "{\n"
            << Space(8) << "\"time\": " << edge.weight.bus_time << ",\n"
            << Space(8) << "\"bus\": " << '\"' << edge.weight.bus_name << "\",\n"
            << Space(8) << "\"span_count\": " << edge.weight.count << ",\n"
            << Space(8) << "\"type\": " << "\"Bus\"\n";
          bus_elems.push_back(BusElement{edge.weight.bus_name, edge.weight.from_idx, edge.weight.to_idx});

          if (i == result.value().edge_count - 1) {
            output_stream << Space(6) << "}\n";
          }
          else {
            output_stream << Space(6) << "},\n";
          }    
        }
        output_stream << Space(4) << "],\n";
      }
      else {
        output_stream << Space(4) << "\"items\": [],\n";
      }

      output_stream
        << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"total_time\": " << result.value().weight.wait_time + result.value().weight.bus_time << ",\n"
        << Space(4) << "\"map\": \"";
      m_paint->RenderRoute(output_stream, bus_elems, wait_elems);
      output_stream << "\"\n";
      
      //m_router->ReleaseRoute(result.value().id);
    }
    else {
      output_stream
        << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"error_message\": " << "\"not found\"\n";
    }
  }

  void StatRequestMapHandle(const Json::Node& route_node, ostream& output_stream = cout) const {
    int64_t id = static_cast<int64_t>(route_node.AsMap().at("id").AsNumber());
    output_stream << Space(4) << "\"request_id\": " << id << ",\n";
    output_stream << Space(4) << "\"map\": \"";
    m_paint->Render(output_stream);
    output_stream << "\"\n";
  }
  
  void BuildGraph() {
    m_graph = make_unique<Graph::DirectedWeightedGraph<GraphWeight>>(m_stops_storage.GetStops().size());
    
    // for not same stops
    for (auto& [_, route] : m_buses) {
      route.BuildGraph(*m_graph);
    }
    
    // for same stops
    for (const auto& [stop_name, stop_info] : m_stops_storage.GetStops()) {
      GraphWeight graph_node;
      graph_node.wait_time = 0;
      graph_node.bus_time = 0.0;
      graph_node.from_stop_name = stop_name;
      graph_node.count = 0;
      
      m_graph->AddEdge(Graph::Edge<GraphWeight>{
        stop_info.number,
        stop_info.number,
        std::move(graph_node)});
    }
    
    m_router = make_unique<Graph::Router<GraphWeight>>(*m_graph);
  }
  
  void BuildPaint() {
    m_paint = make_unique<Paint>(m_render_settings, m_stops_storage, m_buses);
  }

private:
  // Settings
  RenderSettings m_render_settings;

  // Stops and buses
  StopsStorage m_stops_storage;
  list<Route> m_routes;
  map<string_view, const Route&> m_buses;

  // Routing
  unique_ptr<Graph::DirectedWeightedGraph<GraphWeight>> m_graph;
  unique_ptr<Graph::Router<GraphWeight>> m_router;

  // Paint
  unique_ptr<Paint> m_paint;
};

//string input_str;

int main() {
  cout.precision(6);
  
  /*ifstream input("example2.json");
  Json::Document json_doc = Json::Load(input);
  Json::Node root_node = json_doc.GetRoot();

  Svg::SetShielding(false);
  DatabaseManager database_manager;
  database_manager.ReadRoutingSettings(root_node);
  database_manager.ReadRenderSettings(root_node);

  database_manager.UpdateDatabase(root_node);
  database_manager.QueryDatabase(root_node, cout);*/

  /*getline(cin, input_str);
  istringstream input_iss(input_str);
  Json::Document json_doc = Json::Load(input_iss);*/

  Json::Document json_doc = Json::Load(cin);
  Json::Node root_node = json_doc.GetRoot();

  Svg::SetShielding(true);
  DatabaseManager database_manager;
  database_manager.ReadRoutingSettings(root_node);
  database_manager.ReadRenderSettings(root_node);
  database_manager.UpdateDatabase(root_node);
  database_manager.QueryDatabase(root_node, cout);

  return 0;
}
