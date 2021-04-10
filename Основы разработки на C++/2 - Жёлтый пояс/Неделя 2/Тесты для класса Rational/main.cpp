#include <sstream>
#include <exception>
#include <iostream>
#include <string>
#include <map>
#include <set>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
  os << "{";
  bool first = true;
  for (const auto& kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u,
  const string& hint)
{
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u
      << " hint: " << hint;
    throw runtime_error(os.str());
  }
}

inline void Assert(bool b, const string& hint) {
  AssertEqual(b, true, hint);
}

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string& test_name) {
    try {
      func();
      cerr << test_name << " OK" << endl;
    }
    catch (runtime_error& e) {
      ++fail_count;
      cerr << test_name << " fail: " << e.what() << endl;
    }
  }

  ~TestRunner() {
    if (fail_count > 0) {
      cerr << fail_count << " unit tests failed. Terminate" << endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};

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

void TestDefaultCtor()
{
  Rational rat;
  AssertEqual(rat.Numerator(), 0, "");
  AssertEqual(rat.Denominator(), 1, "");
}

void TestMainCtor()
{
  Rational rat1(2, 4);
  AssertEqual(rat1.Numerator(), 1, "");
  AssertEqual(rat1.Denominator(), 2, "");

  Rational rat2(-2, 4);
  AssertEqual(rat2.Numerator(), -1, "");
  AssertEqual(rat2.Denominator(), 2, "");

  Rational rat3(2, -4);
  AssertEqual(rat3.Numerator(), -1, "");
  AssertEqual(rat3.Denominator(), 2, "");

  Rational rat4(-2, -4);
  AssertEqual(rat4.Numerator(), 1, "");
  AssertEqual(rat4.Denominator(), 2, "");

  Rational rat5(0, 4);
  AssertEqual(rat5.Numerator(), 0, "");
  AssertEqual(rat5.Denominator(), 1, "");
}

void TestAll()
{
  TestRunner tr;
  tr.RunTest(TestDefaultCtor, "TestDefaultCtor");
  tr.RunTest(TestMainCtor, "TestMainCtor");
}

int main()
{
  TestAll();
  return 0;
}
