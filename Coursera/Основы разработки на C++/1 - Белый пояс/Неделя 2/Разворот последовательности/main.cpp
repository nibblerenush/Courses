#include <iostream>
#include <vector>

void Reverse(std::vector<int>& v)
{
  for (std::size_t i = 0; i < v.size() / 2; ++i)
  {
    int temp = v[i];
    v[i] = v[v.size() - 1 - i];
    v[v.size() - 1 - i] = temp;
  }
}

int main()
{
  std::vector<int> numbers = { 1, 5, 3, 4, 2 };;
  Reverse(numbers);
  for (auto number : numbers)
  {
    std::cout << number << ' ';
  }
  return 0;
}
