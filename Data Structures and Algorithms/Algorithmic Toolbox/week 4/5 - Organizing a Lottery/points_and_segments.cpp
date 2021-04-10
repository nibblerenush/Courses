#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

vector<int> fast_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
  using namespace std;
  vector<int> cnt(points.size());

  sort(starts.begin(), starts.end());
  sort(ends.begin(), ends.end());

  for (int i = 0; i < (int)points.size(); ++i) {
    auto bigger = upper_bound(starts.begin(), starts.end(), points[i]);
    int bigger_exclude = distance(bigger, starts.end());

    auto less = lower_bound(ends.begin(), ends.end(), points[i]);
    int less_exclude = distance(ends.begin(), less);

    cnt[i] = (int)starts.size() - bigger_exclude - less_exclude;
  }

  return cnt;
}

vector<int> naive_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
  vector<int> cnt(points.size());
  for (size_t i = 0; i < points.size(); i++) {
    for (size_t j = 0; j < starts.size(); j++) {
      cnt[i] += starts[j] <= points[i] && points[i] <= ends[j];
    }
  }
  return cnt;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<int> starts(n), ends(n);
  for (size_t i = 0; i < starts.size(); i++) {
    std::cin >> starts[i] >> ends[i];
  }
  vector<int> points(m);
  for (size_t i = 0; i < points.size(); i++) {
    std::cin >> points[i];
  }
  //use fast_count_segments
  //vector<int> cnt = naive_count_segments(starts, ends, points);
  vector<int> cnt = fast_count_segments(starts, ends, points);
  for (size_t i = 0; i < cnt.size(); i++) {
    std::cout << cnt[i] << ' ';
  }
}
