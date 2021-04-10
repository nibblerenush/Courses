#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

int gcd(int a, int b)
{
  int min{ 0 };
  int max{ 0 };
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

  int mod{ 0 };
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

    if (q == 0)
    {
      throw std::invalid_argument("Invalid argument");
    }

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
  return { res_nom, res_denom };
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
  if (rht.Numerator() == 0)
  {
    throw std::domain_error("Division by zero");
  }

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
  if (stream)
  {
    int num;
    char slash;
    int denom;
    stream >> num >> slash >> denom;
    if (stream && slash == '/')
    {
      Rational temp_rational{ num ,denom };
      rational = temp_rational;
    }
  }
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

int main()
{
  try
  {
    Rational rat1;
    Rational rat2;
    char op;
    std::cin >> rat1 >> op >> rat2;

    switch (op)
    {
    case '+':
      std::cout << rat1 + rat2 << std::endl;
      break;
    case '-':
      std::cout << rat1 - rat2 << std::endl;
      break;
    case '*':
      std::cout << rat1 * rat2 << std::endl;
      break;
    case '/':
      std::cout << rat1 / rat2 << std::endl;
      break;
    }
  }
  catch (const std::invalid_argument& e)
  {
    std::cout << e.what() << std::endl;
  }
  catch (const std::domain_error& e)
  {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
