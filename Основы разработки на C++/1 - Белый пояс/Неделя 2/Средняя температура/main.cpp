#include <iostream>
#include <vector>

int main()
{
  int N;
  std::cin >> N;

  std::vector<int> t_values(N);
  int t_average = 0;
  for (auto& t : t_values)
  {
    std::cin >> t;
    t_average += t;
  }
  t_average /= N;

  std::vector<int> results;
  for (std::size_t i = 0; i < N; ++i)
  {
    if (t_values[i] > t_average)
    {
      results.push_back(i);
    }
  }

  std::cout << results.size() << std::endl;
  for (auto result : results)
  {
    std::cout << result << ' ';
  }

  return 0;
}

