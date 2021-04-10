#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

class ReversibleString
{
public:
  ReversibleString()
  {}

  ReversibleString(const std::string& str)
  {
    m_str = str;
  }

  void Reverse()
  {
    std::reverse(std::begin(m_str), std::end(m_str));
    m_str.size(); std::size_t s;
  }

  std::string ToString() const
  {
    return m_str;
  }

private:
  std::string m_str;
};

int main() {
  ReversibleString s("live");
  s.Reverse();
  cout << s.ToString() << endl;

  s.Reverse();
  const ReversibleString& s_ref = s;
  string tmp = s_ref.ToString();
  cout << tmp << endl;

  ReversibleString empty;
  cout << '"' << empty.ToString() << '"' << endl;

  return 0;
}
