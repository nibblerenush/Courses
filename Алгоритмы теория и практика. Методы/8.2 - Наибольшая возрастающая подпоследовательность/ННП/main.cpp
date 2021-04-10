#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

int main()
{
  int number_count;
  std::cin >> number_count;
  
  std::vector<int> numbers(number_count);
  for (int i = 0; i < number_count; ++i) {
    std::cin >> numbers[i];
  }
  
  std::vector<int> d(number_count + 1, -1);
  d[0] = std::numeric_limits<int>::max();
  
  int length = 0;

  std::vector<int> pos(number_count + 1);
  pos[0] = -1;

  std::vector<int> prev(number_count + 1);
  prev[0] = -1;
  
  for (int i = 0; i < number_count; ++i)
  {
    auto iter = std::upper_bound(d.begin(), d.end(), numbers[i], [](int lhs, int rhs) {
      return lhs > rhs;
    });
    
    int d_idx = iter - d.begin();
    
    if (d[d_idx - 1] >= numbers[i] && numbers[i] >= d[d_idx])
    {
      d[d_idx] = numbers[i];
      pos[d_idx] = i;
      prev[i] = pos[d_idx - 1];
      length = std::max(length, d_idx);
    }
  }
  
  std::vector<int> answers;
  answers.reserve(length);

  int ans_idx = pos[length];
  while (ans_idx != -1)
  {
    answers.push_back(ans_idx);
    ans_idx = prev[ans_idx];
  }

  std::cout << length << '\n';
  for (int i = length - 1; i >= 0; --i) {
    std::cout << answers[i] + 1 << ' ';
  }

  return 0;
}
