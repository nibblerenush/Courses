#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
  std::int32_t N;
  std::cin >> N;

  std::vector<std::int32_t> temps;
  for (std::int32_t counter = 0; counter < N; ++counter)
  {
    std::int32_t t;
    std::cin >> t;
    temps.push_back(t);
  }

  std::int64_t sum{0};
  for (const auto& t : temps)
  {
    sum += t;
  }
  std::int32_t avg = sum / N;
  
  N = std::count_if(std::begin(temps), std::end(temps), [&avg](std::int32_t t) { return t > avg; });
  std::cout << N << std::endl;

  for (std::size_t i = 0; i < temps.size(); ++i)
  {
    if (temps.at(i) > avg)
    {
      std::cout << i << ' ';
    }
  }
  return 0;
}
