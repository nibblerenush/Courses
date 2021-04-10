#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::cin;

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

struct Query {
    string type, name;
    int number;
};

vector<Query> read_queries() {
    int n;
    cin >> n;
    vector<Query> queries(n);
    for (int i = 0; i < n; ++i) {
        cin >> queries[i].type;
        if (queries[i].type == "add")
            cin >> queries[i].number >> queries[i].name;
        else
            cin >> queries[i].number;
    }
    return queries;
}

void write_responses(const vector<string>& result) {
    for (size_t i = 0; i < result.size(); ++i)
        std::cout << result[i] << "\n";
}

vector<string> process_queries(const vector<Query>& queries) {
    vector<string> result;
    // Keep list of all existing (i.e. not deleted yet) contacts.
    TelephoneBook book;
    vector<Query> contacts;

    for (size_t i = 0; i < queries.size(); ++i)
        if (queries[i].type == "add") {
            book.Add(queries[i].number, queries[i].name);

            //bool was_founded = false;
            // if we already have contact with such number,
            // we should rewrite contact's name
            /*for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number) {
                    contacts[j].name = queries[i].name;
                    was_founded = true;
                    break;
                }*/
            // otherwise, just add it
            /*if (!was_founded)
                contacts.push_back(queries[i]);*/
        } else if (queries[i].type == "del") {
            book.Del(queries[i].number);

            /*for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number) {
                    contacts.erase(contacts.begin() + j);
                    break;
                }*/
        } else {
            string response = "not found";
            if (book.IsExists(queries[i].number)) {
              response = book.Find(queries[i].number);
            }

            /*for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number) {
                    response = contacts[j].name;
                    break;
                }*/
            result.push_back(response);
        }
    return result;
}

int main() {
    write_responses(process_queries(read_queries()));
    return 0;
}
