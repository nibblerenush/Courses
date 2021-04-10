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

class Person {
public:
  void ChangeFirstName(int year, const std::string& first_name)
  {
    m_first_names[year] = first_name;
  }

  void ChangeLastName(int year, const std::string& last_name)
  {
    m_last_names[year] = last_name;
  }

  std::string GetFullName(int year)
  {
    auto first_name = std::find_if(std::rbegin(m_first_names), std::rend(m_first_names), [year](const auto& item)
    {
      return item.first <= year;
    });

    auto last_name = std::find_if(std::rbegin(m_last_names), std::rend(m_last_names), [year](const auto& item)
    {
      return item.first <= year;
    });

    if (first_name == std::rend(m_first_names) && last_name == std::rend(m_last_names))
    {
      return "Incognito";
    }
    else if (first_name == std::rend(m_first_names))
    {
      return last_name->second + " with unknown first name";
    }
    else if (last_name == std::rend(m_last_names))
    {
      return first_name->second + " with unknown last name";
    }
    else
    {
      return first_name->second + ' ' + last_name->second;
    }
  }
private:
  std::map<int, std::string> m_first_names;
  std::map<int, std::string> m_last_names;
};

void TestIncognito()
{
  Person person;
  for (const auto& year : { 1900, 1950, 2000 })
  {
    AssertEqual(person.GetFullName(year), "Incognito", "person.GetFullName(year): " + std::to_string(year));
  }

  person.ChangeFirstName(1950, "Polina");
  Assert(person.GetFullName(1949) == "Incognito", "Incognito 1949");
  Assert(person.GetFullName(1950) != "Incognito", "Incognito 1950");
  Assert(person.GetFullName(1951) != "Incognito", "Incognito 1951");

  person.ChangeLastName(1940, "Petrova");
  Assert(person.GetFullName(1939) == "Incognito", "Incognito 1939");
  Assert(person.GetFullName(1940) != "Incognito", "Incognito 1940");
  Assert(person.GetFullName(1941) != "Incognito", "Incognito 1941");
}

void TestFirstName()
{
  Person person;
  person.ChangeFirstName(1950, "Polina");
  Assert(person.GetFullName(1949) == "Incognito", "TestFirstName 1949");
  Assert(person.GetFullName(1950) == "Polina with unknown last name", "TestFirstName 1950");
  Assert(person.GetFullName(1951) == "Polina with unknown last name", "TestFirstName 1951");

  person.ChangeFirstName(1960, "Alice");
  Assert(person.GetFullName(1959) == "Polina with unknown last name", "TestFirstName 1959");
  Assert(person.GetFullName(1960) == "Alice with unknown last name", "TestFirstName 1960");
  Assert(person.GetFullName(1961) == "Alice with unknown last name", "TestFirstName 1961");
}

void TestLastName()
{
  Person person;
  person.ChangeLastName(1950, "Petrova");
  Assert(person.GetFullName(1949) == "Incognito", "TestFirstName 1949");
  Assert(person.GetFullName(1950) == "Petrova with unknown first name", "TestLastName 1950");
  Assert(person.GetFullName(1951) == "Petrova with unknown first name", "TestLastName 1951");

  person.ChangeLastName(1960, "Sergeeva");
  Assert(person.GetFullName(1959) == "Petrova with unknown first name", "TestLastName 1959");
  Assert(person.GetFullName(1960) == "Sergeeva with unknown first name", "TestLastName 1960");
  Assert(person.GetFullName(1961) == "Sergeeva with unknown first name", "TestLastName 1961");
}

void TestFirstLastName()
{
  Person person;
  person.ChangeFirstName(1950, "Polina");
  person.ChangeLastName(1950, "Petrova");
  Assert(person.GetFullName(1949) == "Incognito", "TestFirstLastName 1949");
  Assert(person.GetFullName(1950) == "Polina Petrova", "TestFirstLastName 1950");
  Assert(person.GetFullName(1951) == "Polina Petrova", "TestFirstLastName 1951");

  person.ChangeFirstName(1960, "Alice");
  Assert(person.GetFullName(1959) == "Polina Petrova", "TestFirstLastName 1959");
  Assert(person.GetFullName(1960) == "Alice Petrova", "TestFirstLastName 1960");
  Assert(person.GetFullName(1961) == "Alice Petrova", "TestFirstLastName 1961");

  person.ChangeLastName(1970, "Sergeeva");
  Assert(person.GetFullName(1969) == "Alice Petrova", "TestFirstLastName 1969");
  Assert(person.GetFullName(1970) == "Alice Sergeeva", "TestFirstLastName 1970");
  Assert(person.GetFullName(1971) == "Alice Sergeeva", "TestFirstLastName 1971");
}

void TestAll()
{
  TestRunner tr;
  tr.RunTest(TestIncognito, "TestIncognito");
  tr.RunTest(TestFirstName, "TestFirstName");
  tr.RunTest(TestLastName, "TestLastName");
  tr.RunTest(TestFirstLastName, "TestFirstLastName");
}

int main()
{
  TestAll();
  return 0;
}
