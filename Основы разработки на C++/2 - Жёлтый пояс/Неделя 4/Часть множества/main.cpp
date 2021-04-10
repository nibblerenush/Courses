#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

template <typename T>
std::vector<T> FindGreaterElements(const std::set<T>& elements, const T& border)
{
  auto iter = std::find_if(elements.begin(), elements.end(), [&border](const T& element)
  {
    return element > border;
  });

  std::vector<T> results;
  while (iter != elements.end())
  {
    results.push_back(*iter++);
  }

  return results;
}

int main() {
  for (int x : FindGreaterElements(std::set<int>{1, 5, 7, 8}, 5)) {
    std::cout << x << " ";
  }
  std::cout << std::endl;

  std::string to_find = "Python";
  std::cout << FindGreaterElements(std::set<std::string>{"C", "C++"}, to_find).size() << std::endl;
  return 0;
}
