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

// IntCons

template <int H, typename IL>
struct IntCons;

template <int H, int ... T>
struct IntCons<H, IntList<T ...>> {
  using type = IntList<H, T ...>;
};

// Zip

template <typename IL1, typename IL2, template<int, int> typename Func>
struct Zip {
  static int const value = Func<IL1::Head, IL2::Head>::value;
  using type = typename IntCons<value, typename Zip<typename IL1::Tail, typename IL2::Tail, Func>::type>::type;
};

template <template<int, int> typename Func>
struct Zip<IntList<>, IntList<>, Func> {
  using type = IntList<>;
};

// Dimension

template<int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0, int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;

// Quantity

template <typename IL>
class Quantity {
public:
  Quantity() : m_value(0.0) {};

  explicit Quantity(double value) : m_value(value) {};

  double value() const {
    return m_value;
  }

private:
  double m_value;
};

template <typename D>
Quantity<D> operator+ (const Quantity<D>& left, const Quantity<D>& right) {
  return Quantity<D>(left.value() + right.value());
}

template <typename D>
Quantity<D> operator- (const Quantity<D>& left, const Quantity<D>& right) {
  return Quantity<D>(left.value() - right.value());
}

template<int a, int b>
struct Plus {
  static int const value = a + b;
};

template <typename D1, typename D2>
Quantity<typename Zip<D1, D2, Plus>::type> operator* (const Quantity<D1>& left, const Quantity<D2>& right) {
  return Quantity<typename Zip<D1, D2, Plus>::type>(left.value() * right.value());
}

template<int a, int b>
struct Minus {
  static int const value = a - b;
};

template <typename D1, typename D2>
Quantity<typename Zip<D1, D2, Minus>::type> operator/ (const Quantity<D1>& left, const Quantity<D2>& right) {
  return Quantity<typename Zip<D1, D2, Minus>::type>(left.value() / right.value());
}

template <typename D>
Quantity<D> operator* (const Quantity<D>& left, double right) {
  return Quantity<D>(left.value() * right);
}

template <typename D>
Quantity<D> operator/ (const Quantity<D>& left, double right) {
  return Quantity<D>(left.value() / right);
}

template <typename D>
Quantity<D> operator* (double left, const Quantity<D>& right) {
  return Quantity<D>(left * right.value());
}

using Inversion = Dimension<-1, -1, -1, -1, -1, -1, -1>;

template<int a, int b>
struct Mult {
  static int const value = a * b;
};

template <typename D>
Quantity<typename Zip<D, Inversion, Mult>::type> operator/ (double left, const Quantity<D>& right) {
  return Quantity<typename Zip<D, Inversion, Mult>::type>(left / right.value());
}

using NumberQ = Quantity<Dimension<>>;           // число без размерности
using LengthQ = Quantity<Dimension<1>>;          // метры
using MassQ = Quantity<Dimension<0, 1>>;         // килограммы
using TimeQ = Quantity<Dimension<0, 0, 1>>;      // секунды
using VelocityQ = Quantity<Dimension<1, 0, -1>>; // метры в секунду
using AccelQ = Quantity<Dimension<1, 0, -2>>;    // ускорение, метры в секунду в квадрате
using ForceQ = Quantity<Dimension<1, 1, -2>>;    // сила в ньютонах

int main() {
  LengthQ l{ 30000 }; // 30 км
  TimeQ t{ 10 * 60 }; // 10 минут

  // вычисление скорости
  VelocityQ v = l / t; // результат типа VelocityQ, 50 м/с
  
  AccelQ a{ 9.8 }; // ускорение свободного падения
  MassQ m{ 80 }; // 80 кг

  // сила притяжения, которая действует на тело массой 80 кг
  ForceQ f = m * a; // результат типа ForceQ

  using Q1 = LengthQ;
  using Q2 = MassQ;
  NumberQ n = ((Q1(3) * Q2(4)) / ((2.0 / Q1(2)) * Q2(2) * Q1(2) * Q1(1)));

  return 0;
}
