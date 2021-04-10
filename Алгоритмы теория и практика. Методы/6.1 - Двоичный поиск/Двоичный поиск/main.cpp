#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

int get_pos(const std::vector<int>& numbers, int number)
{
  int l = 0;
  int r = static_cast<int>(numbers.size()) - 1;

  while (l <= r)
  {
    std::size_t m = (l + r) / 2;
    if (numbers[m] == number)
    {
      return m + 1;
    }
    else if (numbers[m] > number)
    {
      r = m - 1;
    }
    else
    {
      l = m + 1;
    }
  }
  return -1;
}

int main(void) {
  size_t number_count;
  std::cin >> number_count;
  std::vector<int> numbers(number_count);
  for (size_t i = 0; i < number_count; i++) {
    std::cin >> numbers[i];
  }

  size_t query_count;
  std::cin >> query_count;
  while (query_count-- > 0) {
    int number;
    std::cin >> number;
    std::cout << get_pos(numbers, number) << " ";
  }
  std::cout << std::endl;
}
