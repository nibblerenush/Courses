#include "profile.h"
#include "test_runner.h"

#include "Bus.h"
#include "GeoCoords.h"
#include "json.h"
#include "Paint.h"
#include "StopsStorage.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <unordered_set>

#include "graph.h"
#include "router.h"

using namespace std;

string Space(int number) {
  return string(number, ' ');
}

class DatabaseManager {
public:
  void ReadRoutingSettings(const Json::Node& root_node)
  {
    // routing_settings
    const Json::Node& routing_settings = root_node.AsMap().at("routing_settings");
    m_routing_settings.bus_wait_time = static_cast<int>(routing_settings.AsMap().at("bus_wait_time").AsNumber());
    m_routing_settings.bus_velocity = routing_settings.AsMap().at("bus_velocity").AsNumber() * 1000.0 / 60.0;
    
    // render_settings
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
  }

  void UpdateDatabase(const Json::Node& root_node)
  {
    // Load root node
    const Json::Node& base_requests = root_node.AsMap().at("base_requests");

    vector<const Json::Node*> postponed_requests;
    for (const Json::Node& base_request : base_requests.AsArray())
    {
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
  }

  void QueryDatabase(const Json::Node& root_node, ostream& output_stream = cout) const
  {
    // Load root node
    const Json::Node& stat_requests = root_node.AsMap().at("stat_requests");

    output_stream << "[\n";
    bool first = true;
    for (const Json::Node& stat_request : stat_requests.AsArray())
    {
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
  void BaseRequestStopHandle(const Json::Node& stop_node)
  {
    const string& stop_name = stop_node.AsMap().at("name").AsString();
    GeoCoords stop_geo_coords = {
      stop_node.AsMap().at("latitude").AsNumber(),
      stop_node.AsMap().at("longitude").AsNumber()
    };
    m_stops_storage.Add(stop_name, move(stop_geo_coords));

    if (stop_node.AsMap().count("road_distances"))
    {
      for (const auto& road_distance : stop_node.AsMap().at("road_distances").AsMap())
      {
        m_stops_storage.AddAdjacentStop(stop_name, road_distance.first, static_cast<int>(road_distance.second.AsNumber()));
      }
    }
  }

  void BaseRequestBusHandle(const Json::Node* bus_node)
  {
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
    route.SetRoutingSettings(m_routing_settings);

    bus.SetRoute(route);
    m_buses.emplace(bus.GetNumber(), move(bus));
  }

  void StatRequestBusHandle(const Json::Node& bus_node, ostream& output_stream = cout) const
  {
    const string& bus_number = bus_node.AsMap().at("name").AsString();
    int64_t id = static_cast<int64_t>(bus_node.AsMap().at("id").AsNumber());

    if (m_buses.count(bus_number))
    {
      const Bus& bus = m_buses.at(bus_number);
      const Route& route = bus.GetRoute();

      output_stream << Space(4) << "\"route_length\": " << route.GetDistance() << ",\n"
        << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"curvature\": " << route.GetCurvature() << ",\n"
        << Space(4) << "\"stop_count\": " << route.GetStopsOnRoute() << ",\n"
        << Space(4) << "\"unique_stop_count\": " << route.GetUniqueStops() << '\n';
    }
    else
    {
      output_stream << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"error_message\": " << "\"not found\"\n";
    }
  }

  void StatRequestStopHandle(const Json::Node& stop_node, ostream& output_stream = cout) const
  {
    const string& stop_name = stop_node.AsMap().at("name").AsString();
    int64_t id = static_cast<int64_t>(stop_node.AsMap().at("id").AsNumber());

    optional<const set<string>*> buses = m_stops_storage.GetBuses(stop_name);
    if (!buses.has_value())
    {
      output_stream << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"error_message\": " << "\"not found\"\n";
    }
    else if (buses.value()->empty())
    {
      output_stream << Space(4) << "\"buses\": [],\n"
        << Space(4) << "\"request_id\": " << id << '\n';
    }
    else
    {
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
      output_stream << '\n' << Space(4) << "],\n"
        << Space(4) << "\"request_id\": " << id << '\n';
    }
  }

  void StatRequestRouteHandle(const Json::Node& route_node, ostream& output_stream = cout) const
  {
    int64_t id = static_cast<int64_t>(route_node.AsMap().at("id").AsNumber());
    const string& from = route_node.AsMap().at("from").AsString();
    const string& to = route_node.AsMap().at("to").AsString();
    auto result = m_router->BuildRoute(m_stops_storage.GetStopInfo(from).number, m_stops_storage.GetStopInfo(to).number);
    
    if (result.has_value())
    {
      if (result.value().edge_count != 0)
      {
        output_stream << Space(4) << "\"items\": [\n";
        bool first = true;
        for (std::size_t i = 0; i < result.value().edge_count; ++i)
        {
          Graph::EdgeId edgeId = m_router->GetRouteEdge(result.value().id, i);
          const Graph::Edge<GraphWeight>& edge = m_graph->GetEdge(edgeId);

          if (edge.weight.wait_time != 0)
          {
            output_stream << Space(6) << "{\n"
              << Space(8) << "\"time\": " << edge.weight.wait_time << ",\n"
              << Space(8) << "\"stop_name\": " << '\"' << edge.weight.from_stop_name << "\",\n"
              << Space(8) << "\"type\": " << "\"Wait\"\n"
              << Space(6) << "},\n";
          }

          output_stream << Space(6) << "{\n"
            << Space(8) << "\"time\": " << edge.weight.bus_time << ",\n"
            << Space(8) << "\"bus\": " << '\"' << edge.weight.bus_name << "\",\n"
            << Space(8) << "\"span_count\": " << edge.weight.count << ",\n"
            << Space(8) << "\"type\": " << "\"Bus\"\n";

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

      output_stream << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"total_time\": " << result.value().weight.wait_time + result.value().weight.bus_time << '\n';

      m_router->ReleaseRoute(result.value().id);
    }
    else
    {
      output_stream << Space(4) << "\"request_id\": " << id << ",\n"
        << Space(4) << "\"error_message\": " << "\"not found\"\n";
    }
  }

  void StatRequestMapHandle(const Json::Node& route_node, ostream& output_stream = cout) const
  {
    int64_t id = static_cast<int64_t>(route_node.AsMap().at("id").AsNumber());
    output_stream << Space(4) << "\"request_id\": " << id << ",\n";
    output_stream << Space(4) << "\"map\": \"";

    Paint paint(m_render_settings, m_stops_storage, m_buses);
    paint.Draw(output_stream);

    output_stream << "\"\n";
  }
  
  void BuildGraph()
  {
    m_graph = make_unique<Graph::DirectedWeightedGraph<GraphWeight>>(m_stops_storage.GetSize());
    
    for (auto [key, value] : m_buses) {
      value.GetRoute().SetBusName(key);
      value.GetRoute().BuildGraph(*m_graph);
    }
    
    m_router = make_unique<Graph::Router<GraphWeight>>(*m_graph);
  }

private:
  RoutingSettings m_routing_settings;
  RenderSettings m_render_settings;
  StopsStorage m_stops_storage;
  map<string, Bus> m_buses;
  unique_ptr<Graph::DirectedWeightedGraph<GraphWeight>> m_graph;
  unique_ptr<Graph::Router<GraphWeight>> m_router;
};

int main()
{
  cout.precision(6);
  
  /*ifstream input("example_one.json");
  Json::Document json_doc = Json::Load(input);
  Json::Node root_node = json_doc.GetRoot();
  
  DatabaseManager database_manager;
  database_manager.ReadRoutingSettings(root_node);
  database_manager.UpdateDatabase(root_node);
  database_manager.QueryDatabase(root_node, cout);*/

  /*auto input_str = R"({"base_requests": [{"type": "Stop", "name": "Tolstopaltsevo", "latitude": 55.611087, "longitude": 37.20829, "road_distances": {"Marushkino": 3900}}, {"type": "Stop", "name": "Marushkino", "latitude": 55.595884, "longitude": 37.209755, "road_distances": {"Rasskazovka": 9900}}, {"type": "Bus", "name": "256", "stops": ["Biryulyovo Zapadnoye", "Biryusinka", "Universam", "Biryulyovo Tovarnaya", "Biryulyovo Passazhirskaya", "Biryulyovo Zapadnoye"], "is_roundtrip": true}, {"type": "Bus", "name": "750", "stops": ["Tolstopaltsevo", "Marushkino", "Rasskazovka"], "is_roundtrip": false}, {"type": "Stop", "name": "Rasskazovka", "latitude": 55.632761, "longitude": 37.333324, "road_distances": {}}, {"type": "Stop", "name": "Biryulyovo Zapadnoye", "latitude": 55.574371, "longitude": 37.6517, "road_distances": {"Biryusinka": 1800, "Universam": 2400, "Rossoshanskaya ulitsa": 7500}}, {"type": "Stop", "name": "Biryusinka", "latitude": 55.581065, "longitude": 37.64839, "road_distances": {"Universam": 750}}, {"type": "Stop", "name": "Universam", "latitude": 55.587655, "longitude": 37.645687, "road_distances": {"Biryulyovo Tovarnaya": 900, "Rossoshanskaya ulitsa": 5600}}, {"type": "Stop", "name": "Biryulyovo Tovarnaya", "latitude": 55.592028, "longitude": 37.653656, "road_distances": {"Biryulyovo Passazhirskaya": 1300}}, {"type": "Stop", "name": "Biryulyovo Passazhirskaya", "latitude": 55.580999, "longitude": 37.659164, "road_distances": {"Biryulyovo Zapadnoye": 1200}}, {"type": "Bus", "name": "828", "stops": ["Biryulyovo Zapadnoye", "Universam", "Rossoshanskaya ulitsa", "Biryulyovo Zapadnoye"], "is_roundtrip": true}, {"type": "Stop", "name": "Rossoshanskaya ulitsa", "latitude": 55.595579, "longitude": 37.605757, "road_distances": {}}, {"type": "Stop", "name": "Prazhskaya", "latitude": 55.611678, "longitude": 37.603831, "road_distances": {}}], "stat_requests": [{"id": 1400184887, "type": "Bus", "name": "256"}, {"id": 1496400948, "type": "Bus", "name": "750"}, {"id": 1219009996, "type": "Bus", "name": "751"}, {"id": 1218841164, "type": "Stop", "name": "Samara"}, {"id": 1486189096, "type": "Stop", "name": "Prazhskaya"}, {"id": 598551223, "type": "Stop", "name": "Biryulyovo Zapadnoye"}]})";
  stringstream ss(move(input_str));

  Json::Document json_doc = Json::Load(ss);
  Json::Node root_node = json_doc.GetRoot();
  
  DatabaseManager database_manager;
  database_manager.ReadRoutingSettings(root_node);
  database_manager.UpdateDatabase(root_node);
  database_manager.QueryDatabase(root_node, cout);*/

  Json::Document json_doc = Json::Load(cin);
  Json::Node root_node = json_doc.GetRoot();

  DatabaseManager database_manager;
  database_manager.ReadRoutingSettings(root_node);
  database_manager.UpdateDatabase(root_node);
  database_manager.QueryDatabase(root_node, cout);

  return 0;
}
