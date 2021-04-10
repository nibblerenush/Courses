#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

int main()
{
  int N;
  std::cin >> N;

  std::vector<int> numbers(N);
  for (auto& number : numbers)
  {
    std::cin >> number;
  }

  std::sort(std::begin(numbers), std::end(numbers), [](const int& a, const int& b)
  {
    if (std::abs(a) < std::abs(b))
    {
      return true;
    }
    return false;
  });

  for (const auto& number : numbers)
  {
    std::cout << number << ' ';
  }
  std::cout << std::endl;

  return 0;
}
