#include <iostream>

template <int ... Numbers>
struct IntList;

template <int H, int ... T>
struct IntList<H, T ...> {
  static const int Head = H;
  using Tail = IntList<T ...>;
};

template <>
struct IntList<> {};

template <typename IL>
struct Length {
  static const int value = 1 + Length<typename IL::Tail>::value;
};

template <>
struct Length<IntList<>> {
  static const int value = 0;
};

int main() {
  using primes = IntList<2, 3, 5, 7, 11, 13>;

  constexpr size_t len = Length<primes>::value;

  std::cout << len << std::endl;
  return 0;
}
