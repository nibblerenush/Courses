#include "profile.h"
#include "test_runner.h"

#include "Bus.h"
#include "GeoCoords.h"
#include "json.h"
#include "Paint.h"
#include "StopsStorage.h"
#include "transport_catalog.pb.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>

#include "graph.h"
#include "router.h"

using namespace std;

string Space(int number) {
  return string(number, ' ');
}

struct SerializationSettings {
  string file;
};

struct BusResponse {
  int route_length;
  double curvature;
  size_t stop_count;
  size_t unique_stop_count;
};

struct StopResponse {
  set<string> buses;
};

class DatabaseManager {
public:
  void ReadRoutingSettings(const Json::Node& root_node) {
    const Json::Node& routing_settings = root_node.AsMap().at("routing_settings");
    m_routing_settings.bus_wait_time = static_cast<int>(routing_settings.AsMap().at("bus_wait_time").AsNumber());
    m_routing_settings.bus_velocity = routing_settings.AsMap().at("bus_velocity").AsNumber() * 1000.0 / 60.0;    
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

    auto get_color = [](const Json::Node& node) -> Svg::Color
    {
      try {
        return node.AsString();
      }
      catch (std::bad_variant_access&)
      {
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

  void ReadSerializationSettings(const Json::Node& root_node) {
    const Json::Node& serialization_settings = root_node.AsMap().at("serialization_settings");
    m_serialization_settings.file = serialization_settings.AsMap().at("file").AsString();
  }

  void UpdateDatabase(const Json::Node& root_node) {
    // Load root node
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

    //BuildGraph();
    //BuildPaint();
  }

  void Serialize() {
    Serialize::TransportCatalog transport_catalog;

    // bus responses
    for (const auto& [bus_name, bus] : m_buses) {
      const Route& route = bus.GetRoute();
      
      Serialize::BusResponse bus_response;
      bus_response.set_bus_name(bus_name);
      bus_response.set_route_length(route.GetDistance());
      bus_response.set_curvature(route.GetCurvature());
      bus_response.set_stop_count(route.GetStopsOnRoute());
      bus_response.set_unique_stop_count(route.GetUniqueStops());

      *transport_catalog.add_bus_responses() = bus_response;
    }

    // stop responses
    for (const auto& stop : m_stops_storage.GetStops()) {
      Serialize::StopResponse stop_response;
      stop_response.set_stop_name(stop.first);
      for (const auto& bus_name : stop.second.buses) {
        *stop_response.add_buses() = bus_name;
      }

      *transport_catalog.add_stop_responses() = stop_response;
    }

    std::ofstream output_file(m_serialization_settings.file, std::ios_base::binary);
    transport_catalog.SerializeToOstream(&output_file);
  }

  void Deserialize() {
    Serialize::TransportCatalog transport_catalog;
    std::ifstream input_file(m_serialization_settings.file, std::ios_base::binary);

    if (transport_catalog.ParseFromIstream(&input_file)) {
      // bus responses
      for (const auto& proto_bus_response : transport_catalog.bus_responses()) {
        BusResponse bus_response;
        bus_response.route_length = proto_bus_response.route_length();
        bus_response.curvature = proto_bus_response.curvature();
        bus_response.stop_count = proto_bus_response.stop_count();
        bus_response.unique_stop_count = proto_bus_response.unique_stop_count();
        m_bus_reponses.emplace(proto_bus_response.bus_name(), move(bus_response));
      }

      // stop responses
      for (const auto& proto_stop_response : transport_catalog.stop_responses()) {
        StopResponse stop_response;
        for (const auto& bus_name : proto_stop_response.buses()) {
          stop_response.buses.insert(bus_name);
        }
        m_stop_responses.emplace(proto_stop_response.stop_name(), move(stop_response));
      }
    }
    else {
      throw std::runtime_error("Deserialize");
    }
  }

  void QueryDatabase(const Json::Node& root_node, ostream& output_stream = cout) const {
    // Load root node
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
    const string& stop_name = stop_node.AsMap().at("name").AsString();
    GeoCoords stop_geo_coords = {
      stop_node.AsMap().at("latitude").AsNumber(),
      stop_node.AsMap().at("longitude").AsNumber()
    };
    m_stops_storage.Add(stop_name, move(stop_geo_coords));
    
    if (stop_node.AsMap().count("road_distances")) {
      for (const auto& road_distance : stop_node.AsMap().at("road_distances").AsMap()) {
        m_stops_storage.AddAdjacentStop(stop_name, road_distance.first, static_cast<int>(road_distance.second.AsNumber()));
      }
    }
  }
  
  void BaseRequestBusHandle(const Json::Node* bus_node) {
    const string& bus_number = bus_node->AsMap().at("name").AsString();
    Bus bus(bus_number);
    
    Route route;
    for (const Json::Node& stop : bus_node->AsMap().at("stops").AsArray()) {
      const string& stop_name = stop.AsString();
      route.AddStop(stop_name, m_stops_storage);
      m_stops_storage.AddBus(stop_name, bus.GetNumber());
    }
    
    bool is_roundtrip = bus_node->AsMap().at("is_roundtrip").AsBool();
    if (is_roundtrip) {
      route.SetType(RouteType::CIRCLE);
    }
    else {
      route.SetType(RouteType::LINEAR);
    }
    route.SetBusName(bus.GetNumber());
    route.SetRoutingSettings(m_routing_settings);

    bus.SetRoute(route);
    m_buses.emplace(bus.GetNumber(), move(bus));
  }

  void StatRequestBusHandle(const Json::Node& bus_node, ostream& output_stream = cout) const {
    const string& bus_number = bus_node.AsMap().at("name").AsString();
    int64_t id = static_cast<int64_t>(bus_node.AsMap().at("id").AsNumber());

    if (m_bus_reponses.count(bus_number)) {
      const BusResponse& bus_response = m_bus_reponses.at(bus_number);
      output_stream
        << Space(4) << "\"route_length\": " << bus_response.route_length << ",\n"
        << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"curvature\": " << bus_response.curvature << ",\n"
        << Space(4) << "\"stop_count\": " << bus_response.stop_count << ",\n"
        << Space(4) << "\"unique_stop_count\": " << bus_response.unique_stop_count << '\n';
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

    if (!m_stop_responses.count(stop_name)) {
      output_stream
        << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"error_message\": " << "\"not found\"\n";
    }
    else if (m_stop_responses.at(stop_name).buses.empty()) {
      output_stream
        << Space(4) << "\"buses\": [],\n"
        << Space(4) << "\"request_id\": " << id << '\n';
    }
    else {
      output_stream << Space(4) << "\"buses\": [\n";
      bool first = true;
      for (const auto& bus : m_stop_responses.at(stop_name).buses) {
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
  
  void StatRequestRouteHandle(const Json::Node& route_node, ostream& output_stream = cout) const
  {
    int64_t id = static_cast<int64_t>(route_node.AsMap().at("id").AsNumber());
    const string& from = route_node.AsMap().at("from").AsString();
    const string& to = route_node.AsMap().at("to").AsString();
    auto result = m_router->BuildRoute(m_stops_storage.GetStopInfo(from).number, m_stops_storage.GetStopInfo(to).number);
    
    // Help for paint
    std::vector<BusElement> bus_elems;
    std::vector<TimeElement> time_elems;

    if (result.has_value())
    {
      if (result.value().edge_count != 0)
      {
        output_stream << Space(4) << "\"items\": [\n";
        for (std::size_t i = 0; i < result.value().edge_count; ++i) {
          Graph::EdgeId edgeId = m_router->GetRouteEdge(result.value().id, i);
          const Graph::Edge<GraphWeight>& edge = m_graph->GetEdge(edgeId);

          if (edge.weight.wait_time != 0)
          {
            output_stream << Space(6) << "{\n"
              << Space(8) << "\"time\": " << edge.weight.wait_time << ",\n"
              << Space(8) << "\"stop_name\": " << '\"' << edge.weight.from_stop_name << "\",\n"
              << Space(8) << "\"type\": " << "\"Wait\"\n"
              << Space(6) << "},\n";
            time_elems.push_back(TimeElement{ edge.weight.from_stop_name });
          }

          output_stream << Space(6) << "{\n"
            << Space(8) << "\"time\": " << edge.weight.bus_time << ",\n"
            << Space(8) << "\"bus\": " << '\"' << edge.weight.bus_name << "\",\n"
            << Space(8) << "\"span_count\": " << edge.weight.count << ",\n"
            << Space(8) << "\"type\": " << "\"Bus\"\n";
          bus_elems.push_back(BusElement{ edge.weight.bus_name, edge.weight.from_iter, edge.weight.to_iter });

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
      m_paint->RenderRoute(output_stream, bus_elems, time_elems);
      output_stream << "\"\n";

      m_router->ReleaseRoute(result.value().id);
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
    m_graph = make_unique<Graph::DirectedWeightedGraph<GraphWeight>>(m_stops_storage.GetSize());
    for (auto& [key, value] : m_buses) {
      value.GetRoute().BuildGraph(*m_graph);
    }
    m_router = make_unique<Graph::Router<GraphWeight>>(*m_graph);
  }

  void BuildPaint() {
    m_paint = make_unique<Paint>(m_render_settings, m_stops_storage, m_buses);
  }

private:
  // common
  StopsStorage m_stops_storage;
  map<string, Bus> m_buses;

  // routing
  RoutingSettings m_routing_settings;
  unique_ptr<Graph::Router<GraphWeight>> m_router;
  unique_ptr<Graph::DirectedWeightedGraph<GraphWeight>> m_graph;

  // render
  RenderSettings m_render_settings;
  unique_ptr<Paint> m_paint;

  // serialization
  SerializationSettings m_serialization_settings;
  unordered_map<string, BusResponse> m_bus_reponses;
  unordered_map<string, StopResponse> m_stop_responses;
};

//string input_str;

int main(int argc, const char* argv []) {
  if (argc != 2) {
    cerr << "Usage: transport_catalog_part_o [make_base|process_requests]\n";
    return EXIT_FAILURE;
  }
  
  const string_view mode(argv[1]);
  
  Json::Document json_doc = Json::Load(cin);
  Json::Node root_node = json_doc.GetRoot();

  DatabaseManager database_manager;
  database_manager.ReadSerializationSettings(root_node);

  if (mode == "make_base") {
    database_manager.UpdateDatabase(root_node);
    database_manager.Serialize();
  }
  else if (mode == "process_requests") {
    cout.precision(6);
    Svg::SetShielding(true);

    database_manager.Deserialize();
    database_manager.QueryDatabase(root_node, cout);
  }
  
  /*ifstream input("example1.json");
  Json::Document json_doc = Json::Load(input);
  Json::Node root_node = json_doc.GetRoot();
  
  Svg::SetShielding(false);

  DatabaseManager database_manager;
  database_manager.ReadSerializationSettings(root_node);

  database_manager.UpdateDatabase(root_node);
  database_manager.Serialize();

  database_manager.Deserialize();
  database_manager.QueryDatabase(root_node, cout);*/

  /*getline(cin, input_str);
  istringstream input_iss(input_str);
  Json::Document json_doc = Json::Load(input_iss);*/

  return EXIT_SUCCESS;
}
