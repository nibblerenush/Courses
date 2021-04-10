#include <iostream>

int main()
{
  int a, b;
  std::cin >> a >> b;

  int min, max;
  if (a > b)
  {
    max = a;
    min = b;
  }
  else
  {
    max = b;
    min = a;
  }
  
  int mod;
  do
  {
    mod = max % min;
    max = min;
    min = mod;
  } while (mod != 0);

  std::cout << max;
  return 0;
}
