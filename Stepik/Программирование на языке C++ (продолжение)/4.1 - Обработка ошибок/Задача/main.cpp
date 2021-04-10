#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <stdexcept>
#include <sstream>
#include <string>

// описание класса исключения bad_from_string
class bad_from_string : public std::exception {
public:
  bad_from_string(const std::string& msg) : m_msg(msg)
  {}

  bad_from_string(const char* msg) : m_msg(msg)
  {}
  
  const char* what() const noexcept override {
    return m_msg.c_str();
  }

private:
  std::string m_msg;
};

// функция from_string
template<class T>
T from_string(std::string const& s) {
  T result;
  std::istringstream iss(s);
  iss >> std::noskipws >> result;
  if (iss.fail() || iss.peek() != EOF) {
    throw bad_from_string("");
  }
  return result;
}

int main() {
  std::string s1("123");
  int    a1 = from_string<int>(s1); // a1 = 123
  double b1 = from_string<double>(s1); // b1 = 123.0
  std::string c1 = from_string<std::string>(s1); // c1 = "123"

  try {
    std::string s2("12.3");
    //int    a2 = from_string<int>(s2); // исключение
    double b2 = from_string<double>(s2); // b2 = 12.3
    std::string c2 = from_string<std::string>(s2); // c2 = "12.3"

    std::string s3("abc");
    //int    a3 = from_string<int>(s3); // исключение
    //double b3 = from_string<double>(s3); // исключение
    std::string c3 = from_string<std::string>(s3); // c3 = "abc"
  }
  catch (bad_from_string& bfs) {
    std::cout << "ex\n";
  }

  return 0;
}
