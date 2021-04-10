#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include <utility>

class ReadingManager
{
public:
  ReadingManager(): m_user_page(MAX_USER_COUNT + 1, 0), m_page_rating(MAX_PAGE_COUNT + 1, 0)
  {}
  
  void Read(int user, int page)
  {
    if (m_user_page[user] > 0)
    {
      const int& old_page = m_user_page[user];
      --m_page_user_count[old_page];
    }
    else
    {
      ++m_all_users;
    }
    
    ++m_page_user_count[page];
    m_user_page[user] = page;
    
    for (auto iter = std::next(m_page_user_count.begin()); iter != m_page_user_count.end(); iter = std::next(iter))
    {
      m_page_rating[iter->first] = m_page_rating[std::prev(iter)->first] + std::prev(iter)->second;
    }
  }
  
  double Cheer(int user) const
  {
    if (m_user_page[user] == 0)
    {
      return 0;
    }

    if (m_all_users == 1)
    {
      return 1;
    }
    
    const int& readed_page = m_user_page[user];
    const int& rating = m_page_rating[readed_page];
    
    return (rating) * 1.0 / (m_all_users - 1);
  }

private:
  static const int MAX_USER_COUNT = 100'000;
  static const int MAX_PAGE_COUNT = 1000;
  
  std::vector<int> m_user_page;
  std::map<int, int> m_page_user_count;
  std::vector<int> m_page_rating;
  int m_all_users = 0;
};

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  
  ReadingManager manager;

  int query_count;
  std::cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id)
  {
    std::string query_type;
    std::cin >> query_type;
    int user_id;
    std::cin >> user_id;

    if (query_type == "READ")
    {
      int page_count;
      std::cin >> page_count;
      manager.Read(user_id, page_count);
    }
    else if (query_type == "CHEER")
    {
      std::cout << std::setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }
  
  return 0;
}
