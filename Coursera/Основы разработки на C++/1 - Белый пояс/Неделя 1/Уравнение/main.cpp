#include <cmath>
#include <iostream>

int main()
{
  double a;
  double b;
  double c;

  std::cin >> a >> b >> c;

  if (a != 0)
  {
    double D = b * b - 4 * a * c;
    if (D > 0)
    {
      double x1 = (-b + std::sqrt(D)) / (2 * a);
      double x2 = (-b - std::sqrt(D)) / (2 * a);
      std::cout << x1 << ' ' << x2;
    }
    else if (D == 0)
    {
      double x = (-b) / (2 * a);
      std::cout << x;
    }
  }
  else if (b != 0)
  {
    double x = (-c) / b;
    std::cout << x;
  }

  return 0;
}
