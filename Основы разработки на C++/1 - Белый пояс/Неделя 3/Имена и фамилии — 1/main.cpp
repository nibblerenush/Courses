#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

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
