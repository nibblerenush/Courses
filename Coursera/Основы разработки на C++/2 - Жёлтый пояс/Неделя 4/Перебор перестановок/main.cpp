#include "test_runner.h"

#include <algorithm>
#include <iostream>

void TestAll()
{
  TestRunner tr;
}

int main()
{
  TestAll();

  int N;
  std::cin >> N;

  std::vector<int> v;
  for (int i = N; i >= 1; --i)
  {
    v.push_back(i);
  }

  do
  {
    for (const auto& elem : v)
    {
      std::cout << elem << ' ';
    }
    std::cout << std::endl;
  } while (std::prev_permutation(v.begin(), v.end()));

  return 0;
}
