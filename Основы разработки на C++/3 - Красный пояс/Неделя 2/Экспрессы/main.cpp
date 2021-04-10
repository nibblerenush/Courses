#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border)
{
  auto range = numbers.equal_range(border);
  if (range.first == numbers.end())
  {
    return prev(range.first);
  }
  else if (range.first == numbers.begin() || range.first != range.second)
  {
    return range.first;
  }

  int diff1 = abs(border - *prev(range.first));
  int diff2 = abs(border - *range.first);
  return diff1 <= diff2 ? prev(range.first) : range.first;
}

class RouteManager {
public:
  void AddRoute(int start, int finish) {
    reachable_lists_[start].insert(finish);
    reachable_lists_[finish].insert(start);
  }

  int FindNearestFinish(int start, int finish) const {
    int result = abs(start - finish);
    if (reachable_lists_.count(start) < 1) {
      return result;
    }
    const set<int>& reachable_stations = reachable_lists_.at(start);
    if (!reachable_stations.empty()) {
      result = min(result, abs(finish - *FindNearestElement(reachable_stations, finish)));
    }
    return result;
  }
private:
  map<int, set<int>> reachable_lists_;
};

int main() {
  RouteManager routes;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int start, finish;
    cin >> start >> finish;
    if (query_type == "ADD") {
      routes.AddRoute(start, finish);
    }
    else if (query_type == "GO") {
      cout << routes.FindNearestFinish(start, finish) << "\n";
    }
  }

  return 0;
}
