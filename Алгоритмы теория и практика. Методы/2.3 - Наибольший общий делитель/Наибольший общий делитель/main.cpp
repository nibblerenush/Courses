#include <cassert>
#include <iostream>

template <class Int>
Int gcd(Int a, Int b) {
  if (a == 0) {
    return b;
  }

  if (b == 0) {
    return a;
  }

  if (a >= b) {
    return gcd(a % b, b);
  }

  if (b > a) {
    return gcd(a, b % a);
  }
}

int main(void) {
  int a, b;
  std::cin >> a >> b;
  std::cout << gcd(a, b) << std::endl;
  return 0;
}
