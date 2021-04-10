#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class TelephoneBook
{
public:
  TelephoneBook() : m_book(BOOK_SIZE)
  {}

  void Add(int number, std::string name) {
    m_book[number] = std::move(name);
  }

  bool IsExists(int number) const {
    return !m_book[number].empty();
  }

  const std::string& Find(int number) const {
    return m_book[number];
  }

  void Del(int number) {
    m_book[number].clear();
  }

private:
  std::vector<std::string> m_book;
  static const int BOOK_SIZE = 10'000'000;
};

int main()
{
  int queries_count;
  std::cin >> queries_count;

  TelephoneBook book;
  std::ostringstream output;
  for(int i = 0; i < queries_count; ++i)
  {
    std::string query;
    std::cin >> query;

    int number;
    std::cin >> number;

    if (query == "add")
    {
      std::string name;
      std::cin >> name;
      book.Add(number, std::move(name));
    }
    else if (query == "find")
    {
      if (book.IsExists(number)) {
        output << book.Find(number) << '\n';
      }
      else {
        output << "not found\n";
      }
    }
    else if (query == "del") {
      book.Del(number);
    }
  }

  std::cout << output.str();
  return 0;
}
