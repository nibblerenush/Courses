#include "database.h"
#include <stdexcept>

DateEvent::DateEvent(const Date& date, const std::string& event): m_date(date), m_event(event)
{}

const Date& DateEvent::GetDate() const
{
  return m_date;
}

const std::string& DateEvent::GetEvent() const
{
  return m_event;
}

std::ostream& operator << (std::ostream& stream, const DateEvent& date_event)
{
  return stream << date_event.GetDate() << ' ' << date_event.GetEvent();
}

void Database::Add(const Date& date, const std::string& event)
{
  if (!m_set_recs.count(date) || !m_set_recs[date].count(event))
  {
    m_set_recs[date].insert(event);
    m_vec_recs[date].push_back(event);
  }
}

void Database::Print(std::ostream& out) const
{
  for (const auto& record : m_vec_recs)
  {
    for (const auto& event : record.second)
    {
      DateEvent date_event(record.first, event);
      out << date_event << std::endl;
    }
  }
}

DateEvent Database::Last(const Date& date) const
{
  auto iter = m_vec_recs.upper_bound(date);
  if (iter != m_vec_recs.begin())
  {
    return {std::prev(iter)->first, std::prev(iter)->second.back()};
  }
  else
  {
    throw std::invalid_argument("");
  }
}
