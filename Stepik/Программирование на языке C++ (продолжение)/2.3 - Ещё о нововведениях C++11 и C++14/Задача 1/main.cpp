#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

template <int i, int j, typename T>
auto to_pair(const T& tup) -> decltype(std::make_pair(std::get<i>(tup), std::get<j>(tup))) {
  return std::make_pair(std::get<i>(tup), std::get<j>(tup));
}

int main() {
  auto t = std::make_tuple(0, 3.5, "Hello");
  std::pair<double, char const*> p = to_pair<1, 2>(t);
  std::cout << p.first << '\n' << p.second << std::endl;
  return 0;
}
