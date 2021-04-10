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
  
  Rational& operator+=(const Rational& rat) {
    add(rat);
    return *this;
  }

  Rational& operator-=(const Rational& rat) {
    sub(rat);
    return *this;
  }

  Rational& operator*=(const Rational& rat) {
    mul(rat);
    return *this;
  }

  Rational& operator/=(const Rational& rat) {
    div(rat);
    return *this;
  }

  Rational operator+() const {
    Rational rat(numerator_, denominator_);
    return rat;
  }
  
  Rational operator-() const {
    Rational rat(numerator_, denominator_);
    rat.neg();
    return rat;
  }

  void neg();
  void inv();
  double to_double() const;

private:
  int numerator_;
  int denominator_;
};

int main() {
  return 0;
}
