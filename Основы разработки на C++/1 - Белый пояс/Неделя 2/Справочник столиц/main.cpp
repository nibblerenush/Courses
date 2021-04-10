#include <iostream>
#include <map>
#include <string>

void ChangeCapital(std::map<std::string, std::string>& county_capital, const std::string& country, const std::string& new_capital)
{
  if (county_capital.count(country) == 0)
  {
    county_capital[country] = new_capital;
    std::cout << "Introduce new country " << country << " with capital " << new_capital << std::endl;
  }
  else if (county_capital[country] == new_capital)
  {
    std::cout << "Country " << country << " hasn\'t changed its capital" << std::endl;
  }
  else
  {
    std::string old_capital = county_capital[country];
    county_capital[country] = new_capital;
    std::cout << "Country " << country << " has changed its capital from " << old_capital << " to " << new_capital << std::endl;
  }
}

void Rename(std::map<std::string, std::string>& county_capital, const std::string& old_country_name, const std::string& new_country_name)
{
  if (old_country_name == new_country_name || county_capital.count(old_country_name) == 0 || county_capital.count(new_country_name) != 0)
  {
    std::cout << "Incorrect rename, skip" << std::endl;
  }
  else
  {
    std::string capital = county_capital[old_country_name];
    county_capital.erase(old_country_name);
    county_capital[new_country_name] = capital;
    std::cout << "Country " << old_country_name << " with capital " << capital << " has been renamed to " << new_country_name << std::endl;
  }
}

void About(const std::map<std::string, std::string>& county_capital, const std::string& country)
{
  if (county_capital.count(country) == 0)
  {
    std::cout << "Country " << country << " doesn\'t exist" << std::endl;
  }
  else
  {
    std::cout << "Country " << country << " has capital " << county_capital.at(country) << std::endl;
  }
}

void Dump(const std::map<std::string, std::string>& county_capital)
{
  if (county_capital.size() == 0)
  {
    std::cout << "There are no countries in the world" << std::endl;
  }
  else
  {
    for (const auto& item : county_capital)
    {
      std::cout << item.first << '/' << item.second << ' ';
    }
    std::cout << std::endl;
  }
}

int main()
{
  const std::string CHANGE_CAPITAL = "CHANGE_CAPITAL";
  const std::string RENAME = "RENAME";
  const std::string ABOUT = "ABOUT";
  const std::string DUMP = "DUMP";

  std::map<std::string, std::string> county_capital;

  int Q;
  std::cin >> Q;

  for (int i = 0; i < Q; ++i)
  {
    std::string command;
    std::cin >> command;

    if (command == CHANGE_CAPITAL)
    {
      std::string country, new_capital;
      std::cin >> country >> new_capital;
      ChangeCapital(county_capital, country, new_capital);
    }
    else if (command == RENAME)
    {
      std::string old_country_name, new_country_name;
      std::cin >> old_country_name >> new_country_name;
      Rename(county_capital, old_country_name, new_country_name);
    }
    else if (command == ABOUT)
    {
      std::string country;
      std::cin >> country;
      About(county_capital, country);
    }
    else if (command == DUMP)
    {
      Dump(county_capital);
    }
  }

  return 0;
}
