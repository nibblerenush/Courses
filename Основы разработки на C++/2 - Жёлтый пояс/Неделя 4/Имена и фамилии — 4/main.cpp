#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

std::string GetName(const std::map<int, std::string>& names, int year)
{
  auto iter = names.upper_bound(year);
  std::string name;
  if (iter != names.begin())
  {
    name = std::prev(iter)->second;
  }
  return name;
}

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
    std::string first_name = GetName(m_first_names, year);
    std::string last_name = GetName(m_last_names, year);

    if (first_name.empty() && last_name.empty())
    {
      return "Incognito";
    }
    else if (first_name.empty())
    {
      return last_name + " with unknown first name";
    }
    else if (last_name.empty())
    {
      return first_name + " with unknown last name";
    }
    else
    {
      return first_name + ' ' + last_name;
    }
  }
private:
  std::map<int, std::string> m_first_names;
  std::map<int, std::string> m_last_names;
};

int main() {
  Person person;

  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  for (int year : {1900, 1965, 1990}) {
    cout << person.GetFullName(year) << endl;
  }

  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    cout << person.GetFullName(year) << endl;
  }

  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    cout << person.GetFullName(year) << endl;
  }

  return 0;
}
