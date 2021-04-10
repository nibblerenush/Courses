#include <iostream>

void UpdateIfGreater(int first, int & second)
{
  if (first > second)
  {
    second = first;
  }
}

int main()
{
  int a, b;
  std::cin >> a >> b;
  UpdateIfGreater(a, b);
  std::cout << a << ' ' << b;
  return 0;
}
