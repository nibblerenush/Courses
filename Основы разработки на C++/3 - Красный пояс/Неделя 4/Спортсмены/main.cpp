#include "profile.h"
#include "test_runner.h"

#include <iostream>
#include <list>
#include <map>

class Sportsmen
{
public:
  void Add(int num, int before_num)
  {
    if (m_num_pos.count(before_num))
    {
      auto iter = m_line.insert(m_num_pos[before_num], num);
      m_num_pos[num] = iter;
    }
    else
    {
      m_line.push_back(num);
      m_num_pos[num] = std::prev(m_line.end());
    }
  }

  void Print() const
  {
    for (const auto& num : m_line)
    {
      std::cout << num << ' ';
    }
  }
private:
  std::list<int> m_line;
  std::map<int, std::list<int>::iterator> m_num_pos;
};

void TestAll()
{
  TestRunner tr;
}

int main()
{
  TestAll();

  Sportsmen sportsmen;

  int n;
  std::cin >> n;

  for (int counter = 0; counter < n; ++counter)
  {
    int num, before_num;
    std::cin >> num >> before_num;
    sportsmen.Add(num, before_num);
  }
  sportsmen.Print();

  return 0;
}
