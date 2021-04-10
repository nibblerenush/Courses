#include "descriptions.h"
#include "json.h"
#include "requests.h"
#include "sphere.h"
#include "transport_catalog.h"
#include "utils.h"

#include <fstream>
#include <iostream>

using namespace std;

int main() {
  /*std::ifstream file("input1.txt");
  
  if (!file.is_open()) {
    std::cerr << "Not open file\n";
    return 1;
  }
  const auto input_doc = Json::Load(file);*/


  const auto input_doc = Json::Load(cin);
  const auto& input_map = input_doc.GetRoot().AsMap();

  const TransportCatalog db(
    Descriptions::ReadDescriptions(input_map.at("base_requests").AsArray()),
    input_map.at("routing_settings").AsMap()
  );

  Json::PrintValue(
    Requests::ProcessAll(db, input_map.at("stat_requests").AsArray()),
    cout
  );
  cout << endl;

  return 0;
}
