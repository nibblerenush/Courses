#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
  int number_count;
  std::cin >> number_count;

  std::vector<int> numbers(number_count);
  for (int i = 0; i < number_count; ++i) {
    std::cin >> numbers[i];
  }

  std::vector<int> results(number_count, 1);
  for (int i = 1; i < number_count; ++i)
  {
    for (int j = 0; j < i; ++j)
    {
      if (numbers[i] % numbers[j] == 0 && results[j] + 1 > results[i])
      {
        results[i] = results[j] + 1;
      }
    }
  }

  int answer = 1;
  for (const auto& result : results)
  {
    answer = std::max(result, answer);
  }
  std::cout << answer;
  return 0;
}
