#include <fstream>
#include <iostream>
#include <iomanip>

int main()
{
  std::ifstream input("input.txt");
  if (input.is_open())
  {
    std::cout << std::fixed << std::setprecision(3);
    double value;
    while (input >> value)
    {
      std::cout << value << std::endl;
    }
  }

  return 0;
}
