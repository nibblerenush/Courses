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

void print_values(std::ostream& os)
{}

template <typename T, typename ... Args>
void print_values(std::ostream& os, const T& value, const Args& ... args) {
  os << typeid(value).name() << ": " << value << '\n';
  print_values(os, args ...);
}

int main() {
  print_values(std::cout, 0, 3.5, "Hello");
  return 0;
}
