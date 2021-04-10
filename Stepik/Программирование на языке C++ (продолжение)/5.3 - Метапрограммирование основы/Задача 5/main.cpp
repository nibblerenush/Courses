#include <iostream>
#include <tuple>

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

// IntPros

template <int H, typename IL>
struct IntPros;

template <int H, int ... T>
struct IntPros<H, IntList<T ...>> {
  using type = IntList<T..., H>;
};

// Generate

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

// apply

template <typename Func, typename ... Types, int ... Numbers>
typename std::result_of<Func(Types...)>::type
helper(Func func, std::tuple<Types...> work_tuple, IntList<Numbers...>) {
  return func(std::get<Numbers>(work_tuple) ...);
}

template <typename Func, typename ... Types>
typename std::result_of<Func(Types...)>::type
apply(Func func, std::tuple<Types...> work_tuple) {
  typename Generate<sizeof...(Types)>::type positions{};
  return helper(func, work_tuple, positions);
}

int main() {
  auto f = [](int x, double y, double z) { return x + y + z; };
  auto t = std::make_tuple(30, 5.0, 1.6);
  auto res = apply(f, t);

  std::cout << res << std::endl;
  return 0;
}
