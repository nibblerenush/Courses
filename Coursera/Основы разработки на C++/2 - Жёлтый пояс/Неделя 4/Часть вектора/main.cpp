#include <algorithm>
#include <iostream>
#include <vector>

void PrintVectorPart(const std::vector<int>& numbers)
{
  auto iter = std::find_if(numbers.begin(), numbers.end(), [](int number)
  {
    return number < 0;
  });

  while (iter != numbers.begin())
  {
    --iter;
    std::cout << *iter << ' ';
  }
}

int main()
{
  PrintVectorPart({ 6, 1, 8, -5, 4 });
  std::cout << std::endl;
  PrintVectorPart({ -6, 1, 8, -5, 4 });  // ничего не выведется
  std::cout << std::endl;
  PrintVectorPart({ 6, 1, 8, 5, 4 });
  std::cout << std::endl;
  return 0;
}
