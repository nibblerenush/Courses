#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>

using std::vector;

struct Segment {
  int start, end;
};

vector<int> optimal_points(vector<Segment> &segments) {
  using namespace std;

  // preprocessing
  sort(segments.begin(), segments.end(), [](const Segment& seg_left, const Segment& seg_right) {
    return seg_left.end < seg_right.end;
  });

  // algorithm
  int i = 0;
  vector<int> points;

  while (i < (int)segments.size()) {
    int point = segments[i].end;
    while (i < (int)segments.size() && point >= segments[i].start && point <= segments[i].end) {
      i += 1;
    }

    points.push_back(point);
  }
  
  return points;
}

int main() {
  int n;
  std::cin >> n;
  vector<Segment> segments(n);
  for (size_t i = 0; i < segments.size(); ++i) {
    std::cin >> segments[i].start >> segments[i].end;
  }
  vector<int> points = optimal_points(segments);
  std::cout << points.size() << "\n";
  for (size_t i = 0; i < points.size(); ++i) {
    std::cout << points[i] << " ";
  }
}
