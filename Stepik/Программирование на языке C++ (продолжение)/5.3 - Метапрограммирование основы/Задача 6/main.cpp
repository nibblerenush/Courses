#include <iostream>

// IntList

template <int ... Numbers>
struct IntList;

template <int H, int ... T>
struct IntList<H, T ...> {
  static const int Head = H;
  using Tail = IntList<T ...>;
};

template <>
struct IntList<> {};

template <int H, typename IL>
struct IntCons;

template <int H, int ... T>
struct IntCons<H, IntList<T ...>> {
  using type = IntList<H, T ...>;
};

template<int a, int b>
struct Plus {
  static int const value = a + b;
};

template <typename IL1, typename IL2, template<int, int> typename Func>
struct Zip {
  static int const value = Func<IL1::Head, IL2::Head>::value;
  using type = typename IntCons<value, typename Zip<typename IL1::Tail, typename IL2::Tail, Func>::type>::type;
};

template <template<int, int> typename Func>
struct Zip<IntList<>, IntList<>, Func> {
  using type = IntList<>;
};

template <typename IL>
void PrintIntList() {
  std::cout << IL::Head << ' ';
  PrintIntList<typename IL::Tail>();
}

template <>
void PrintIntList<IntList<>>() {
  std::cout << std::endl;
};

int main() {
  using L1 = IntList<1, 2, 3, 4, 5>;
  PrintIntList<L1>();

  using L2 = IntList<1, 3, 7, 7, 2>;
  PrintIntList<L2>();

  using L3 = Zip<L1, L2, Plus>::type;
  PrintIntList<L3>();

  return 0;
}
