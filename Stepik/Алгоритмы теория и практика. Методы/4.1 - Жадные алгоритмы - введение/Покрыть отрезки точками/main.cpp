#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

typedef std::pair<int, int> Segment;

std::vector<int> get_covering_set(std::vector<Segment> segments)
{
  // Sort segments
  std::sort(segments.begin(), segments.end());
  
  // Main algorithm
  std::vector<int> result;
  int cur_point = segments[0].second;
  for (int i = 1; i < segments.size(); ++i)
  {
    if (segments[i].first <= cur_point)
    {
      if (segments[i].second < cur_point)
      {
        cur_point = segments[i].second;
      }
    }
    else
    {
      result.push_back(cur_point);
      cur_point = segments[i].second;
    }
  }
  result.push_back(cur_point);

  return result;
}

int main(void) {
  int segments_count;
  std::cin >> segments_count;
  std::vector <Segment> segments(segments_count);
  for (int i = 0; i < segments_count; i++) {
    std::cin >> segments[i].first >> segments[i].second;
  }

  std::vector <int> points = get_covering_set(segments);
  std::cout << points.size() << std::endl;
  for (std::size_t i = 0; i < points.size(); i++) {
    std::cout << points[i] << " ";
  }
  std::cout << std::endl;
}
