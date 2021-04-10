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

template <typename T, typename U>
struct SameType {
  static const bool value = false;
};

template <typename T>
struct SameType<T, T> {
  static const bool value = true;
};

struct Dummy { };
typedef int type;

int main() {
  std::cout << SameType<int, int>::value << std::endl; // выведет 1, т. е. true
  std::cout << SameType<int, type>::value << std::endl; // 1, type == int
  std::cout << SameType<int, int&>::value << std::endl; // 0, int и ссылка на int - различные типы
  std::cout << SameType<Dummy, Dummy>::value << std::endl; // 1
  std::cout << SameType<int, const int>::value << std::endl; // 0, const - часть 
  return 0;
}
