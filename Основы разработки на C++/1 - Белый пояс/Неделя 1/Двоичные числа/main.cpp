#include <iostream>
#include <vector>

int main()
{
  int a;
  std::cin >> a;

  std::vector<int> result;
  while (a > 0)
  {
    int mod = a % 2;
    result.push_back(mod);
    a /= 2;
  }

  if (result.size() == 0)
  {
    std::cout << a;
  }
  else
  {
    for (int i = result.size() - 1; i >= 0; --i)
    {
      std::cout << result[i];
    }
  }

  return 0;
}
