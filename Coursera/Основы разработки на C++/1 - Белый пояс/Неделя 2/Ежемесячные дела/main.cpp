#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void Add(std::vector<std::vector<std::string>>& current_deals, const int& i, const std::string& s)
{
  current_deals[i - 1].push_back(s);
}

void Dump(std::vector<std::vector<std::string>>& current_deals, const int& i)
{
  std::cout << current_deals[i - 1].size() << ' ';
  for (auto deal : current_deals[i - 1])
  {
    std::cout << deal << ' ';
  }
  std::cout << std::endl;
}

void Next(const std::vector<int>& month, int& current_month, std::vector<std::vector<std::string>>& current_deals)
{
  int previous_month = current_month;

  if (current_month == 11)
  {
    current_month = 0;
  }
  else
  {
    current_month++;
  }

  if (month[current_month] < month[previous_month])
  {
    int diff = month[previous_month] - month[current_month];
    std::vector<std::string>& last_day_deals = current_deals[month[current_month] - 1];

    for (int i = 0; i < diff; ++i)
    {
      std::vector<std::string>& to_last_day_deals = current_deals[month[current_month] + i];
      last_day_deals.insert(std::end(last_day_deals), std::begin(to_last_day_deals), std::end(to_last_day_deals));
    }
  }
  current_deals.resize(month[current_month]);
}

int main()
{
  const std::string ADD = "ADD";
  const std::string DUMP = "DUMP";
  const std::string NEXT = "NEXT";
  const std::vector<int> months = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  int current_month = 0;
  std::vector<std::vector<std::string>> current_deals(months[current_month]);

  int Q;
  std::cin >> Q;

  for (int counter = 0; counter < Q; ++counter)
  {
    std::string command;
    std::cin >> command;

    if (command == ADD)
    {
      int i;
      std::string s;
      std::cin >> i >> s;
      Add(current_deals, i, s);
    }
    else if (command == DUMP)
    {
      int i;
      std::cin >> i;
      Dump(current_deals, i);
    }
    else if (command == NEXT)
    {
      Next(months, current_month, current_deals);
    }
  }

  return 0;
}
