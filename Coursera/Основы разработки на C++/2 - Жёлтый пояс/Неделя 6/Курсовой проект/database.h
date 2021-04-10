#pragma once

#include "date.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

class DateEvent
{
public:
  DateEvent(const Date& date, const std::string& event);
  const Date& GetDate() const;
  const std::string& GetEvent() const;
private:
  const Date& m_date;
  const std::string& m_event;
};

std::ostream& operator << (std::ostream& stream, const DateEvent& date_event);

class Database
{
public:
  void Add(const Date& date, const std::string& event);
  void Print(std::ostream& out) const;
  template <typename Predicate> int RemoveIf(Predicate pred);
  template <typename Predicate> std::vector<DateEvent> FindIf(Predicate pred) const;
  DateEvent Last(const Date& date) const;
private:
  std::map<Date, std::set<std::string>> m_set_recs;
  std::map<Date, std::vector<std::string>> m_vec_recs;
};

template <typename Predicate>
int Database::RemoveIf(Predicate pred)
{
  int count = 0;
  std::vector<Date> erase_dates;
  
  for (auto& record : m_set_recs)
  {
    std::vector<std::string> erase_events;
    for (const auto& event : record.second)
    {
      if (pred(record.first, event))
      {
        ++count;
        erase_events.push_back(event);
      }
    }

    for (const auto& event : erase_events)
    {
      record.second.erase(event);
    }

    if (record.second.empty())
    {
      erase_dates.push_back(record.first);
    }
  }

  for (auto& record : m_vec_recs)
  {
    auto iter = std::stable_partition(record.second.begin(), record.second.end(), [&record, &pred](const std::string& event)
    {
      return !pred(record.first, event);
    });
    record.second.resize(iter - record.second.begin());
  }

  for (const auto& date : erase_dates)
  {
    m_set_recs.erase(date);
    m_vec_recs.erase(date);
  }

  return count;
}

template <typename Predicate>
std::vector<DateEvent> Database::FindIf(Predicate pred) const
{
  std::vector<DateEvent> results;
  for (const auto& record : m_vec_recs)
  {
    for (const auto& event : record.second)
    {
      if (pred(record.first, event))
      {
        results.push_back({ record.first, event });
      }
    }
  }
  return results;
}
