#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

class Year
{
public:
  explicit Year(int year)
  {
    m_year = year;
  }

  int Get() const
  {
    return m_year;
  }
private:
  int m_year;
};

class Month
{
public:
  explicit Month(int month)
  {
    if (month < 1 || month > 12)
    {
      throw std::invalid_argument("Month value is invalid: " + std::to_string(month));
    }
    m_month = month;
  }

  int Get() const
  {
    return m_month;
  }
private:
  int m_month;
};

class Day
{
public:
  explicit Day(int day)
  {
    if (day < 1 || day > 31)
    {
      throw std::invalid_argument("Day value is invalid: " + std::to_string(day));
    }
    m_day = day;
  }

  int Get() const
  {
    return m_day;
  }
private:
  int m_day;
};

class Date {
public:
  Date(Year year, Month month, Day day)
  {
    m_year = year.Get();
    m_month = month.Get();
    m_day = day.Get();
  }

  int GetYear() const
  {
    return m_year;
  }

  int GetMonth() const
  {
    return m_month;
  }

  int GetDay() const
  {
    return m_day;
  }
private:
  int m_year;
  int m_month;
  int m_day;
};

bool operator<(const Date& lhs, const Date& rhs)
{
  if (lhs.GetYear() == rhs.GetYear())
  {
    if (lhs.GetMonth() == rhs.GetMonth())
    {
      return lhs.GetDay() < rhs.GetDay();
    }
    else
    {
      return lhs.GetMonth() < rhs.GetMonth();
    }
  }
  else
  {
    return lhs.GetYear() < rhs.GetYear();
  }
}

Date GetDate(const std::string& date_str)
{
  int year;
  char hyphen_first;
  int month;
  char hyphen_second;
  int day;
  
  std::istringstream isstream(date_str);
  isstream >> year >> hyphen_first >> month >> hyphen_second >> day;
  if (isstream && hyphen_first == '-' && hyphen_second == '-' && isstream.eof())
  {
    return {Year{year}, Month{month}, Day{day}};
  }
  else
  {
    throw std::invalid_argument("Wrong date format: " + date_str);
  }
}

std::ostream& operator<<(std::ostream& stream, const Date& date)
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

class Database {
public:
  void AddEvent(const Date& date, const std::string& event)
  {
    m_records[date].insert(event);
  }

  bool DeleteEvent(const Date& date, const std::string& event)
  {
    if (m_records.count(date))
    {
      if (m_records[date].count(event))
      {
        m_records[date].erase(event);
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }

  int DeleteDate(const Date& date)
  {
    if (m_records.count(date))
    {
      int N = m_records[date].size();
      m_records.erase(date);
      return N;
    }
    else
    {
      return 0;
    }
  }
  
  void Find(const Date& date) const
  {
    if (m_records.count(date))
    {
      for (const auto& event : m_records.at(date))
      {
        std::cout << event << std::endl;
      }
    }
  }

  void Print() const
  {
    for (const auto& record : m_records)
    {
      for (const auto& event : record.second)
      {
        if (record.first.GetYear() >= 0)
        {
          std::cout << record.first << ' ' << event << std::endl;
        }
      }
    }
  }
private:
  std::map<Date, std::set<std::string>> m_records;
};

int main()
{
  Database db;
  std::string command;
  try
  {
    while (std::getline(std::cin, command))
    {
      if (!command.empty())
      {
        std::istringstream isstream(command);
        std::string command_name;
        isstream >> command_name;

        if (command_name == "Add")
        {
          std::string date_str;
          std::string event;
          isstream >> date_str >> event;
          db.AddEvent(GetDate(date_str), event);
        }
        else if (command_name == "Del")
        {
          std::string date_str;
          isstream >> date_str;
          
          if (!isstream.eof())
          {
            std::string event;
            isstream >> event;

            if (db.DeleteEvent(GetDate(date_str), event))
            {
              std::cout << "Deleted successfully" << std::endl;
            }
            else
            {
              std::cout << "Event not found" << std::endl;
            }
          }
          else
          {
            int count = db.DeleteDate(GetDate(date_str));
            std::cout << "Deleted " << count << " events" << std::endl;
          }
        }
        else if (command_name == "Find")
        {
          std::string date_str;
          isstream >> date_str;
          db.Find(GetDate(date_str));
        }
        else if (command_name == "Print")
        {
          db.Print();
        }
        else
        {
          throw std::domain_error("Unknown command: " + command_name);
        }
      }
    }
  }
  catch (const std::logic_error& ex)
  {
    std::cout << ex.what() << std::endl;
  }
  return 0;
}
