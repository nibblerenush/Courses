#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

std::string GetChronoOrder(const std::map<int, std::string>& names, int year)
{
  std::vector<std::string> out_strs;

  std::string current_str;
  for (const auto& ln : names)
  {
    if (ln.first < year)
    {
      if (ln.second != current_str)
      {
        current_str = ln.second;
        out_strs.push_back(current_str);
      }
    }
    else
    {
      break;
    }
  }

  std::string result;
  if (!out_strs.empty())
  {
    if (out_strs[out_strs.size() - 1] == names.at(year))
    {
      out_strs.pop_back();
    }
    
    if (!out_strs.empty())
    {
      for (int i = out_strs.size() - 1; i >= 0; --i)
      {
        result += out_strs[i] + ", ";
      }
      
      result.pop_back();
      result.pop_back();
    }
  }

  return result;
}

std::string GetResultOutput(const std::string& name, const std::string& chrono_names)
{
  if (chrono_names.empty())
  {
    return name;
  }
  return name + " (" + chrono_names + ')';
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

  std::string GetFullNameWithHistory(int year)
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
      return GetResultOutput(last_name->second, GetChronoOrder(m_last_names, last_name->first)) + " with unknown first name";
    }
    else if (last_name == std::rend(m_last_names))
    {
      return GetResultOutput(first_name->second, GetChronoOrder(m_first_names, first_name->first)) + " with unknown last name";
    }
    else
    {
      return GetResultOutput(first_name->second, GetChronoOrder(m_first_names, first_name->first)) +
             ' ' +
             GetResultOutput(last_name->second, GetChronoOrder(m_last_names, last_name->first));
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
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeFirstName(1990, "Polina");
  person.ChangeLastName(1990, "Volkova-Sergeeva");
  cout << person.GetFullNameWithHistory(1990) << endl;

  person.ChangeFirstName(1966, "Pauline");
  cout << person.GetFullNameWithHistory(1966) << endl;

  person.ChangeLastName(1960, "Sergeeva");
  for (int year : {1960, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeLastName(1961, "Ivanova");
  cout << person.GetFullNameWithHistory(1967) << endl;

  return 0;
}
