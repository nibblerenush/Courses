#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct Rational {
  Rational(int numerator = 0, int denominator = 1);

  void add(Rational rational);
  void sub(Rational rational);
  void mul(Rational rational);
  void div(Rational rational);

  void neg();
  void inv();
  double to_double() const;

  Rational& operator+=(Rational rational);
  Rational& operator-=(Rational rational);
  Rational& operator*=(Rational rational);
  Rational& operator/=(Rational rational);

  Rational operator-() const;
  Rational operator+() const;

private:
  int numerator_;
  int denominator_;
};

Rational operator+(Rational left, const Rational& right) {
  left += right;
  return left;
}

Rational operator-(Rational left, const Rational& right) {
  left -= right;
  return left;
}

Rational operator*(Rational left, const Rational& right) {
  left *= right;
  return left;
}

Rational operator/(Rational left, const Rational& right) {
  left /= right;
  return left;
}

int main() {
  return 0;
}
