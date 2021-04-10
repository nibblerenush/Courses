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
  Person(const std::string& first_name, const std::string& last_name, int born_year)
  {
    m_first_names[born_year] = first_name;
    m_last_names[born_year] = last_name;
    m_born_year = born_year;
  }

  void ChangeFirstName(int year, const std::string& first_name)
  {
    if (year >= m_born_year)
    {
      m_first_names[year] = first_name;
    }
  }

  void ChangeLastName(int year, const std::string& last_name)
  {
    if (year >= m_born_year)
    {
      m_last_names[year] = last_name;
    }
  }

  std::string GetFullName(int year) const
  {
    if (year >= m_born_year)
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
    else
    {
      return "No person";
    }
  }

  std::string GetFullNameWithHistory(int year) const
  {
    if (year >= m_born_year)
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
    else
    {
      return "No person";
    }
  }
private:
  std::map<int, std::string> m_first_names;
  std::map<int, std::string> m_last_names;
  int m_born_year;
};

int main() {
  Person person("Polina", "Sergeeva", 1960);
  for (int year : {1959, 1960}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeFirstName(1965, "Appolinaria");
  person.ChangeLastName(1967, "Ivanova");
  for (int year : {1965, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  return 0;
}
