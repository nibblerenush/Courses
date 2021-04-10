#include <iostream>

int Factorial(int x)
{
  if (x <= 1)
  {
    return 1;
  }
  else
  {
    return x * Factorial(x - 1);
  }
}

int main()
{
  int x;
  std::cin >> x;
  std::cout << Factorial(x);
  return 0;
}
