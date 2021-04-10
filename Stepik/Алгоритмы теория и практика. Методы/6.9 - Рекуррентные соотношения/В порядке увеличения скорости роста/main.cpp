#include <cmath>
#include <iostream>
#include <vector>

struct RecurrentInput
{
  int a;
  int b;
  int d;
};

void Recurrent(RecurrentInput input)
{
  double log_ab = std::log(input.a) / std::log(input.b);
  std::cout << "a = " << input.a << ", b = " << input.b << ", d = " << input.d << ", log_ab = " << log_ab << '\n';

  if (input.d > log_ab) {
    std::cout << "O(n ^ " << input.d << ")\n";
  }
  else if (input.d < log_ab) {
    std::cout << "O(n ^ " << log_ab << ")\n";
  }
  else {
    std::cout << "O(n ^ " << input.d << " * log_n)\n";
  }
}

int main()
{
  std::vector<RecurrentInput> inputs = {
    {6, 4, 3},
    {2, 3, 0},
    {9, 3, 2},
    {2, 2, 1},
    {5, 4, 1},
    {1, 2, 0},
    {3, 2, 1},
    {5, 4, 2},
    {5, 2, 1}
  };

  for (const auto& input : inputs)
  {
    Recurrent(input);
    std::cout << std::endl;
  }

  return 0;
}
