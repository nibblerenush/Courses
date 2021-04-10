#include <iostream>

template <int N>
struct Fib {
  static const int value = Fib<N - 1>::value + Fib<N - 2>::value;
};

template <>
struct Fib<0> {
  static const int value = 0;
};

template <>
struct Fib<1> {
  static const int value = 1;
};

int main() {
  std::cout << Fib<10>::value;
  return 0;
}
