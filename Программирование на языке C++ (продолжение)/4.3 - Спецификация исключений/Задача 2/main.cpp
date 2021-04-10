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

template<class T>
void do_math() noexcept(
  noexcept(T(std::declval<T&>())) &&
  noexcept(std::declval<T&>() = std::declval<T&>()) &&
  noexcept(std::declval<T&>() + std::declval<T&>()) &&
  noexcept(T(std::declval<T>())) &&
  noexcept(std::declval<T&>() = std::declval<T>()))
{
  // тело функции нужно оставить пустым
}

int main() {
  bool b1 = noexcept(do_math<int>()); // true

  bool b2 = noexcept(do_math<std::string>()); // false

  return 0;
}
