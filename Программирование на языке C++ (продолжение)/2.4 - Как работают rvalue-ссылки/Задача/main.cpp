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

template <typename Func, typename ... Args>
auto apply(Func func, Args&& ... args) -> decltype(func(std::forward<Args>(args)...)) {
  return func(std::forward<Args>(args)...);
}

int main() {
  auto fun = [](std::string a, std::string const& b) { return a += b; };

  std::string s("world!");

  // s передаётся по lvalue ссылке,
  // а временный объект по rvalue ссылке 
  s = apply(fun, std::string("Hello, "), s);

  return 0;
}
