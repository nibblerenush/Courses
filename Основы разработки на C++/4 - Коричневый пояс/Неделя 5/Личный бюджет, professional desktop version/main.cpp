#include "profile.h"
#include "test_runner.h"

#include <ctime>
#include <iostream>

class Date
{
public:
  explicit Date() : m_day(1), m_month(1), m_year(2000)
  {}

  explicit Date(int day, int month, int year) : m_day(day), m_month(month), m_year(year)
  {}

  std::time_t AsTimestamp() const
  {
    std::tm t;
    t.tm_sec = 0;
    t.tm_min = 0;
    t.tm_hour = 0;
    t.tm_mday = m_day;
    t.tm_mon = m_month - 1;
    t.tm_year = m_year - 1900;
    t.tm_isdst = 0;
    return std::mktime(&t);
  }

private:
  int m_day;
  int m_month;
  int m_year;
};

std::istream& operator >> (std::istream& input_stream, Date& date)
{
  if (input_stream)
  {
    int year;
    char hyphen_one;
    int month;
    char hyphen_two;
    int day;
    if (input_stream >> year >> hyphen_one >> month >> hyphen_two >> day)
    {
      Date temp_date(day, month, year);
      date = temp_date;
    }
  }
  return input_stream;
}

int ComputeDaysDiff(const Date& date_to, const Date& date_from)
{
  const std::time_t timestamp_to = date_to.AsTimestamp();
  const std::time_t timestamp_from = date_from.AsTimestamp();
  static const int SECONDS_IN_DAY = 60 * 60 * 24;
  return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}

class BudgetManager
{
public:
  BudgetManager() : m_income_in_days(MAX_DAY_COUNT, 0), m_spend_in_days(MAX_DAY_COUNT, 0)
  {}

  void Earn(const Date& date_from, const Date& date_to, int value)
  {
    auto [diff_days, index] = GetVectorIndex(date_from, date_to);
    double per_day_value = value / (static_cast<double>(diff_days) + 1);
    for (int i = 0; i <= diff_days; ++i) {
      m_income_in_days.at(index + i) += per_day_value;
    }
  }

  double ComputeIncome(const Date& date_from, const Date& date_to) const
  {
    auto [diff_days, index] = GetVectorIndex(date_from, date_to);
    double result = 0;
    for (int i = 0; i <= diff_days; ++i) {
      result += (m_income_in_days.at(index + i) - m_spend_in_days.at(index + i));
    }
    return result;
  }

  void PayTax(const Date& date_from, const Date& date_to, int percentage)
  {
    auto [diff_days, index] = GetVectorIndex(date_from, date_to);
    double value_after_tax = (100.0 - percentage) / 100.0;
    for (int i = 0; i <= diff_days; ++i) {
      m_income_in_days.at(index + i) *= value_after_tax;
    }
  }

  void Spend(const Date& date_from, const Date& date_to, int value)
  {
    auto [diff_days, index] = GetVectorIndex(date_from, date_to);
    double per_day_value = value / (static_cast<double>(diff_days) + 1);
    for (int i = 0; i <= diff_days; ++i) {
      m_spend_in_days.at(index + i) += per_day_value;
    }
  }

private:
  struct VectorIndex
  {
    int diff_days;
    int index;
  };

  VectorIndex GetVectorIndex(const Date& date_from, const Date& date_to) const {
    return { ComputeDaysDiff(date_to, date_from), ComputeDaysDiff(date_from, MIN_DATE) };
  }

private:
  std::vector<double> m_income_in_days;
  std::vector<double> m_spend_in_days;
  static const std::size_t MAX_DAY_COUNT;
  static const Date MIN_DATE;
};

const std::size_t BudgetManager::MAX_DAY_COUNT = 36526;
const Date BudgetManager::MIN_DATE = Date(1, 1, 2000);

void TestAll()
{
  TestRunner tr;
}

// 946684800
// 4102444800

int main()
{
  TestAll();

  int Q;
  std::cin >> Q;
  std::cout.precision(25);

  BudgetManager manager;
  for (int counter = 0; counter < Q; ++counter)
  {
    std::string command;
    std::cin >> command;
    if (command == "ComputeIncome")
    {
      Date date_from;
      Date date_to;
      std::cin >> date_from >> date_to;
      std::cout << manager.ComputeIncome(date_from, date_to) << '\n';
    }
    else if (command == "Earn")
    {
      Date date_from;
      Date date_to;
      int value;
      std::cin >> date_from >> date_to >> value;
      manager.Earn(date_from, date_to, value);
    }
    else if (command == "PayTax")
    {
      Date date_from;
      Date date_to;
      int percentage;
      std::cin >> date_from >> date_to >> percentage;
      manager.PayTax(date_from, date_to, percentage);
    }
    else if (command == "Spend")
    {
      Date date_from;
      Date date_to;
      int value;
      std::cin >> date_from >> date_to >> value;
      manager.Spend(date_from, date_to, value);
    }
  }

  return 0;
}
