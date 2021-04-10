#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
  int number_of_segments;
  int number_of_points;
  std::cin >> number_of_segments >> number_of_points;
  
  std::vector<int> begins(number_of_segments);
  std::vector<int> ends(number_of_segments);
  for (int i = 0; i < number_of_segments; ++i)
  {
    std::cin >> begins[i] >> ends[i];
  }
  
  std::sort(begins.begin(), begins.end());
  std::sort(ends.begin(), ends.end());
  
  for (int i = 0; i < number_of_points; ++i)
  {
    int point;
    std::cin >> point;
    
    int result = number_of_segments;

    auto iter = std::upper_bound(begins.begin(), begins.end(), point);
    result -= begins.end() - iter;

    iter = std::lower_bound(ends.begin(), ends.end(), point);
    result -= iter - ends.begin();

    result = result < 0 ? 0 : result;
    std::cout << result << ' ';
  }

  return 0;
}
