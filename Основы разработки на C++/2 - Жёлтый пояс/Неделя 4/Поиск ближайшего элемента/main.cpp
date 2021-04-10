#include "test_runner.h"

#include <cmath>
#include <set>

std::set<int>::const_iterator FindNearestElement(const std::set<int>& numbers, int border)
{
  if (numbers.empty())
  {
    return numbers.end();
  }
  
  auto range = numbers.equal_range(border);
  if (range.first == numbers.end())
  {
    return std::prev(range.first);
  }
  else if (range.first == numbers.begin() || range.first != range.second)
  {
    return range.first;
  }
  
  int diff1 = std::abs(border - *std::prev(range.first));
  int diff2 = std::abs(border - *range.first);
  return diff1 <= diff2 ? std::prev(range.first) : range.first;
}

void TestAll()
{
  TestRunner tr;
}

int main() {
  std::set<int> numbers = { 1, 4, 6 };
  std::cout <<
    *FindNearestElement(numbers, 0) << " " <<
    *FindNearestElement(numbers, 3) << " " <<
    *FindNearestElement(numbers, 5) << " " <<
    *FindNearestElement(numbers, 6) << " " <<
    *FindNearestElement(numbers, 100) << std::endl;

  std::set<int> empty_set;

  std::cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << std::endl;
  return 0;
}
