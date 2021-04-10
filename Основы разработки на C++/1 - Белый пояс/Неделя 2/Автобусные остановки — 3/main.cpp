#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

int main()
{
  std::map<std::set<std::string>, int> stops_bus_number;

  int Q;
  std::cin >> Q;

  int bus_number = 0;
  for (int counter = 0; counter < Q; ++counter)
  {
    int N;
    std::cin >> N;

    std::set<std::string> stops;
    for (int i = 0; i < N; ++i)
    {
      std::string stop;
      std::cin >> stop;
      stops.insert(stop);
    }

    if (!stops_bus_number.count(stops))
    {
      stops_bus_number[stops] = ++bus_number;
      std::cout << "New bus " << bus_number << std::endl;
    }
    else
    {
      std::cout << "Already exists for " << stops_bus_number[stops] << std::endl;
    }
  }

  return 0;
}
