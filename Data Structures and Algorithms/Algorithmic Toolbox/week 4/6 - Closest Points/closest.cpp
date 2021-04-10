#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

using std::vector;
using std::string;
using std::pair;
using std::min;

struct Point {
  int x = 0;
  int y = 0;
};

double distance(const Point& p1, const Point& p2) {
  double x_sub = (p1.x - p2.x) * (p1.x - p2.x);
  double y_sub = (p1.y - p2.y) * (p1.y - p2.y);
  return std::sqrt(x_sub + y_sub);
}

double scan_strip_points(const vector<Point>& strip_points) {
  double d = std::numeric_limits<double>::max();
  for (int i = 0; i < (int)strip_points.size(); ++i) {
    for (int j = i + 1; j < (int)strip_points.size() && (strip_points[j].y - strip_points[i].y) < d; ++j) {
      d = min(d, distance(strip_points[i], strip_points[j]));
    }
  }
  return d;
}

double divide_and_conquer(const vector<Point>& points, int left, int right) {
  if (left >= right) {
    return std::numeric_limits<double>::max();
  }

  if (left + 1 == right) {
    return distance(points[left], points[right]);
  }

  int mid = left + (right - left) / 2;
  double d1 = divide_and_conquer(points, left, mid);
  double d2 = divide_and_conquer(points, mid + 1, right);
  double d = min(d1, d2);
  
  vector<Point> strip_points;
  for (int i = left; i <= right; ++i) {
    if (std::abs(points[i].x - points[mid].x) <= d) {
      strip_points.push_back(points[i]);
    }
  }

  std::sort(strip_points.begin(), strip_points.end(), [](const Point& p1, const Point& p2) {
    return p1.y < p2.y;
  });

  return min(d, scan_strip_points(strip_points));
}

double minimal_distance(vector<int> x, vector<int> y) {
  vector<Point> points(x.size());
  for (int i = 0; i < (int)x.size(); ++i) {
    points[i] = Point{ x[i], y[i] };
  }

  std::sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
    if (p1.x == p2.x) {
      return p1.y < p2.y;
    }
    return p1.x < p2.x;
  });

  return divide_and_conquer(points, 0, points.size() - 1);
}

int main() {
  size_t n;
  std::cin >> n;
  vector<int> x(n);
  vector<int> y(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> x[i] >> y[i];
  }
  std::cout << std::fixed;
  std::cout << std::setprecision(9) << minimal_distance(x, y) << "\n";
}
