#include <cstdint>
#include <iostream>
#include <limits>

void PrintError() {
  std::cout << "Overflow!\n";
}

void PrintResult(std::int64_t result) {
  std::cout << result << '\n';
}

int main()
{
  std::int64_t a = 0;
  std::int64_t b = 0;
  std::cin >> a >> b;

  std::int64_t max = std::numeric_limits<std::int64_t>::max();
  std::int64_t min = std::numeric_limits<std::int64_t>::min();

  std::int64_t valid = 0;
  if (a >= 0 && b >= 0)
  {
    valid = max - a;
    if (b <= valid) {
      PrintResult(a + b);
    }
    else {
      PrintError();
    }
  }
  else if (a < 0 && b < 0)
  {
    valid = min - a;
    if (b >= valid) {
      PrintResult(a + b);
    }
    else {
      PrintError();
    }
  }
  else
  {
    PrintResult(a + b);
  }

  return 0;
}
