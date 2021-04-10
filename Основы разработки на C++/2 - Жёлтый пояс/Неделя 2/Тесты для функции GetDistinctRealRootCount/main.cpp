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

int GetDistinctRealRootCount(double a, double b, double c)
{
  if (a != 0)
  {
    double D = b * b - 4 * a * c;
    if (D > 0)
    {
      return 2;
    }
    else if (D == 0)
    {
      return 1;
    }
  }
  else if (b != 0)
  {
    return 1;
  }
  return 0;
}

void TestQuadratic()
{
  AssertEqual(GetDistinctRealRootCount(1, -8, 12), 2, "1 -8 12");
  AssertEqual(GetDistinctRealRootCount(1, -6, 9), 1, "1 -6 9");
  AssertEqual(GetDistinctRealRootCount(5, 3, 7), 0, "5 3 7");
  
  AssertEqual(GetDistinctRealRootCount(4, 0, -1), 2, "4 0 -1");
  AssertEqual(GetDistinctRealRootCount(4, 0, 1), 0, "4 0 1");

  AssertEqual(GetDistinctRealRootCount(4, 4, 0), 2, "4 4 0");
  AssertEqual(GetDistinctRealRootCount(4, 0, 0), 1, "4 0 0");
}

void TestLinear()
{
  AssertEqual(GetDistinctRealRootCount(0, -8, 8), 1, "0 -8 8");
  AssertEqual(GetDistinctRealRootCount(0, -8, 0), 1, "0 -8 0");
  AssertEqual(GetDistinctRealRootCount(0, 0, -8), 0, "0 0 -8");
}

void TestAll()
{
  TestRunner tr;
  tr.RunTest(TestQuadratic, "TestQuadratic");
  tr.RunTest(TestLinear, "TestLinear");
}

int main()
{
  TestAll();
  return 0;
}
