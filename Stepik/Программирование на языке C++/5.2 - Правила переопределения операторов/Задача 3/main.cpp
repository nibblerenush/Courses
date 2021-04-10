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

  int get_num() const {
    return numerator_;
  }

  int get_denom() const {
    return denominator_;
  }

private:
  int numerator_;
  int denominator_;
};

Rational operator+(Rational lhs, Rational rhs);
Rational operator-(Rational lhs, Rational rhs);
Rational operator*(Rational lhs, Rational rhs);
Rational operator/(Rational lhs, Rational rhs);

bool operator<(Rational lhs, Rational rhs) {
  return lhs.get_num() * rhs.get_denom() < rhs.get_num() * lhs.get_denom();
}

bool operator>(Rational lhs, Rational rhs) {
  return rhs < lhs;
}

bool operator<=(Rational lhs, Rational rhs) {
  return !(lhs > rhs);
}

bool operator>=(Rational lhs, Rational rhs) {
  return !(lhs < rhs);
}

bool operator==(Rational lhs, Rational rhs) {
  return !(lhs < rhs) && !(rhs < lhs);
}

bool operator!=(Rational lhs, Rational rhs) {
  return !(lhs == rhs);
}

int main() {
  return 0;
}
