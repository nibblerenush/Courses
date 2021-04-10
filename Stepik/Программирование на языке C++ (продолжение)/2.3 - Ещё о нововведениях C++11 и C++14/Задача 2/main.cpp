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

struct Point {
  constexpr Point(double x, double y) : x(x), y(y)
  {}
  
  double x = 0;
  double y = 0;
};

constexpr Point operator+(const Point& left, const Point& right) {
  return Point(left.x + right.x, left.y + right.y);
}

constexpr Point operator-(const Point& left, const Point& right) {
  return Point(left.x - right.x, left.y - right.y);
}

constexpr double operator*(const Point& left, const Point& right) {
  return left.x * right.x + left.y * right.y;
}

int main() {
  constexpr size_t size = static_cast<size_t>(Point(2, 4) * Point(4, 3));
  int m[size]; // массив из 20 элементов
  return 0;
}
