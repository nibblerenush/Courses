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

int main() {
  using primes = IntList<2, 3, 5, 7, 11, 13>;

  constexpr int head = primes::Head;

  using odd_primes = primes::Tail;

  std::cout << head << std::endl;
  std::cout << odd_primes::Head << std::endl;

  return 0;
}
