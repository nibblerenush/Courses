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

// IntCons

template <int H, typename IL>
struct IntCons;

template <int H, int ... T>
struct IntCons<H, IntList<T ...>> {
  using type = IntList<H, T ...>;
};

// Generate

template <int H, typename IL>
struct IntPros;

template <int H, int ... T>
struct IntPros<H, IntList<T ...>> {
  using type = IntList<T..., H>;
};

template <int N>
struct Generate {
  using type = typename IntPros<N - 1, typename Generate<N - 1>::type>::type;
};

template <>
struct Generate<1> {
  using type = IntList<0>;
};

template <>
struct Generate<0> {
  using type = IntList<>;
};

int main() {
  using L1 = IntList<2, 3, 4>;

  using L2 = IntPros<1, L1>::type;

  /*std::cout << L2::Head << std::endl;
  std::cout << L2::Tail::Head << std::endl;
  std::cout << L2::Tail::Tail::Head << std::endl;
  std::cout << L2::Tail::Tail::Tail::Head << std::endl;*/

  using L3 = Generate<4>::type;
  std::cout << L3::Head << std::endl;
  std::cout << L3::Tail::Head << std::endl;
  std::cout << L3::Tail::Tail::Head << std::endl;

  return 0;
}
