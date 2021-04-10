#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

int gcd(int a, int b)
{
  int min{0};
  int max{0};
  if (a > b)
  {
    max = a;
    min = b;
  }
  else
  {
    max = b;
    min = a;
  }
  
  int mod{0};
  do
  {
    mod = max % min;
    max = min;
    min = mod;
  } while (mod != 0);

  return max;
}

class Rational
{
public:
  Rational()
  {
    p = 0;
    q = 1;
  }

  Rational(int numerator, int denominator)
  {
    p = numerator;
    q = denominator;

    if (p == 0)
    {
      q = 1;
      return;
    }

    if (q < 0)
    {
      p = -p;
      q = -q;
    }
    
    int pq_gcd = gcd(std::abs(p), q);
    p /= pq_gcd;
    q /= pq_gcd;
  }

  int Numerator() const
  {
    return p;
  }

  int Denominator() const
  {
    return q;
  }

private:
  int p;
  int q;
};

bool operator==(const Rational& lft, const Rational& rht)
{
  return lft.Numerator() == rht.Numerator() && lft.Denominator() == rht.Denominator();
}

Rational operator+(const Rational& lft, const Rational& rht)
{
  int res_denom = lft.Denominator() * rht.Denominator();
  int res_nom = lft.Numerator() * rht.Denominator() + rht.Numerator() * lft.Denominator();
  return {res_nom, res_denom};
}

Rational operator-(const Rational& lft, const Rational& rht)
{
  int res_denom = lft.Denominator() * rht.Denominator();
  int res_nom = lft.Numerator() * rht.Denominator() - rht.Numerator() * lft.Denominator();
  return { res_nom, res_denom };
}

Rational operator*(const Rational& lft, const Rational& rht)
{
  int res_nom = lft.Numerator() * rht.Numerator();
  int res_denom = lft.Denominator() * rht.Denominator();
  return { res_nom, res_denom };
}

Rational operator/(const Rational& lft, const Rational& rht)
{
  int res_nom = lft.Numerator() * rht.Denominator();
  int res_denom = lft.Denominator() * rht.Numerator();
  return { res_nom, res_denom };
}

std::ostream& operator<<(std::ostream& stream, const Rational& rational)
{
  stream << rational.Numerator() << '/' << rational.Denominator();
  return stream;
}

std::istream& operator>>(std::istream& stream, Rational& rational)
{
  int num;
  char slash;
  int denom;

  if (stream.tellg() == -1)
  {
    return stream;
  }
  stream >> num;

  if (stream.tellg() == -1)
  {
    return stream;
  }
  stream >> slash;
  if (slash != '/')
  {
    return stream;
  }

  if (stream.tellg() == -1)
  {
    return stream;
  }
  stream >> denom;

  Rational temp_rational{ num ,denom };
  rational = temp_rational;
  return stream;
}

bool operator<(const Rational& lft, const Rational& rht)
{
  if (lft.Numerator() == rht.Numerator())
  {
    if (lft.Denominator() == rht.Denominator())
    {
      return false;
    }
  }

  double div_lft = static_cast<double>(lft.Numerator()) / lft.Denominator();
  double div_rht = static_cast<double>(rht.Numerator()) / rht.Denominator();
  return div_lft < div_rht;
}

int main() {
  {
    const set<Rational> rs = { {1, 2}, {1, 25}, {3, 4}, {3, 4}, {1, 2} };
    if (rs.size() != 3) {
      cout << "Wrong amount of items in the set" << endl;
      return 1;
    }

    vector<Rational> v;
    for (auto x : rs) {
      v.push_back(x);
    }
    if (v != vector<Rational>{ {1, 25}, { 1, 2 }, { 3, 4 }}) {
      cout << "Rationals comparison works incorrectly" << endl;
      return 2;
    }
  }

  {
    map<Rational, int> count;
    ++count[{1, 2}];
    ++count[{1, 2}];

    ++count[{2, 3}];

    if (count.size() != 2) {
      cout << "Wrong amount of items in the map" << endl;
      return 3;
    }
  }

  cout << "OK" << endl;
  return 0;
}
