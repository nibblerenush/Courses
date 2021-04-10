#pragma once

#include <iostream>

class Date
{
public:
  Date(int year, int month, int day);
  const int& GetYear() const;
  const int& GetMonth() const;
  const int& GetDay() const;
private:
  const int m_year;
  const int m_month;
  const int m_day;
};

bool operator < (const Date& lhs, const Date& rhs);
bool operator > (const Date& lhs, const Date& rhs);
bool operator == (const Date& lhs, const Date& rhs);
bool operator <= (const Date& lhs, const Date& rhs);
bool operator >= (const Date& lhs, const Date& rhs);
bool operator != (const Date& lhs, const Date& rhs);

Date ParseDate(std::istream& stream);
std::ostream& operator << (std::ostream& stream, const Date& date);
