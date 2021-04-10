#pragma once

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// ===== interface =====

template <class T>
std::ostream& operator << (std::ostream& os, const std::set<T>& s);

template <class K, class V>
std::ostream& operator << (std::ostream& os, const std::map<K, V>& m);

template <class K, class V>
std::ostream& operator << (std::ostream& os, const std::unordered_map<K, V>& om);

template <class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v);

template<class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint);

inline void Assert(bool b, const std::string& hint);

class TestRunner
{
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const std::string& test_name);
  ~TestRunner();
private:
  int fail_count = 0;
};

// ===== interface =====

template <class T>
std::ostream& operator << (std::ostream& os, const std::set<T>& s)
{
  os << "{";
  bool first = true;
  for (const auto& x : s)
  {
    if (!first)
    {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
std::ostream& operator << (std::ostream& os, const std::map<K, V>& m)
{
  os << "{";
  bool first = true;
  for (const auto& kv : m)
  {
    if (!first)
    {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template <class K, class V>
std::ostream& operator << (std::ostream& os, const std::unordered_map<K, V>& om)
{
  os << "{";
  bool first = true;
  for (const auto& kv : om)
  {
    if (!first)
    {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template <class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v)
{
  os << "[";
  bool first = true;
  for (const auto& x : v)
  {
    if (!first)
    {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "]";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint)
{
  if (t != u)
  {
    std::ostringstream os;
    os << "Assertion failed: " << t << " != " << u << " hint: " << hint;
    throw std::runtime_error(os.str());
  }
}

inline void Assert(bool b, const std::string& hint)
{
  AssertEqual(b, true, hint);
}

template <class TestFunc>
void TestRunner::RunTest(TestFunc func, const std::string& test_name)
{
  try
  {
    func();
    std::cerr << test_name << " OK" << std::endl;
  }
  catch (std::runtime_error& e)
  {
    ++fail_count;
    std::cerr << test_name << " fail: " << e.what() << std::endl;
  }
  catch (...)
  {
    ++fail_count;
    std::cerr << "Unknown exception caught" << std::endl;
  }
}

#define ASSERT_EQUAL(x, y) \
  AssertEqual(x, y, #x " != " #y ", " __FILE__ ":" + std::to_string(__LINE__))

#define ASSERT(x) \
  Assert(x, #x " is false, " __FILE__ ":" + std::to_string(__LINE__))

#define RUN_TEST(tr, func) \
  tr.RunTest(func, #func)
