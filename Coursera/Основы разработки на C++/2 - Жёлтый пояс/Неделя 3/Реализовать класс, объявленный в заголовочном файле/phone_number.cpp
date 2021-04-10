#include "phone_number.h"
#include <algorithm>
#include <stdexcept>
#include <vector>

PhoneNumber::PhoneNumber(const string& international_number)
{
  if (international_number.empty())
  {
    throw invalid_argument("Number is empty");
  }
  else if (international_number.at(0) != '+')
  {
    throw invalid_argument("Number doesn\'t contain plus");
  }
  
  string result;
  int dash_count = 0;
  vector<string> results;
  for (size_t i = 1; i < international_number.length(); ++i)
  {
    const char& c = international_number.at(i);
    if (dash_count < 2)
    {
      if (c != '-')
      {
        result.push_back(c);
      }
      else
      {
        ++dash_count;
        results.push_back(result);
        result.clear();
      }
    }
    else
    {
      result.push_back(c);
    }
  }
  results.push_back(result);

  if (results.size() != 3)
  {
    throw invalid_argument("Empty part");
  }
  else
  {
    for (const auto& result : results)
    {
      if (result.empty())
      {
        throw invalid_argument("Empty part");
      }
    }
  }

  country_code_ = results.at(0);
  city_code_ = results.at(1);
  local_number_ = results.at(2);
}

string PhoneNumber::GetCountryCode() const
{
  return country_code_;
}

string PhoneNumber::GetCityCode() const
{
  return city_code_;
}

string PhoneNumber::GetLocalNumber() const
{
  return local_number_;
}

string PhoneNumber::GetInternationalNumber() const
{
  return ('+' + country_code_ + '-' + city_code_ + '-' + local_number_);
}
