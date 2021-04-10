#include <iostream>
#include <vector>

std::vector<int> Reversed(const std::vector<int>& v)
{
  std::vector<int> result;
  for (int i = v.size() - 1; i >= 0; --i)
  {
    result.push_back(v[i]);
  }
  return result;
}

int main()
{
  std::vector<int> v = { 1, 5, 3, 4, 2 };
  auto result = Reversed(v);
  return 0;
}
