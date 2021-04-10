#include "date.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>

Date::Date(int year, int month, int day): m_year(year), m_month(month), m_day(day)
{}

const int& Date::GetYear() const
{ return m_year; }

const int& Date::GetMonth() const
{ return m_month; }

const int& Date::GetDay() const
{ return m_day; }

bool operator < (const Date& lhs, const Date& rhs)
{
  return std::tie(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) < std::tie(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool operator > (const Date& lhs, const Date& rhs)
{
  return rhs < lhs;
}

bool operator == (const Date& lhs, const Date& rhs)
{
  return !(lhs < rhs) && !(rhs < lhs);
}

bool operator <= (const Date& lhs, const Date& rhs)
{
  return lhs < rhs || lhs == rhs;
}

bool operator >= (const Date& lhs, const Date& rhs)
{
  return lhs > rhs || lhs == rhs;
}

bool operator != (const Date& lhs, const Date& rhs)
{
  return !(lhs == rhs);
}

Date ParseDate(std::istream& stream)
{
  int year;
  char hyphen_first;
  int month;
  char hyphen_second;
  int day;

  std::string date_str;
  stream >> date_str;

  std::istringstream isstream(date_str);
  isstream >> year >> hyphen_first >> month >> hyphen_second >> day;
  if (isstream && hyphen_first == '-' && hyphen_second == '-' && isstream.eof())
  {
    return { year, month, day };
  }
  else
  {
    throw std::invalid_argument("Wrong date format: " + date_str);
  }
}

std::ostream& operator << (std::ostream& stream, const Date& date)
{
  if (stream)
  {
    stream << std::setfill('0')
      << std::setw(4) << date.GetYear() << '-'
      << std::setw(2) << date.GetMonth() << '-'
      << std::setw(2) << date.GetDay();
  }
  return stream;
}
