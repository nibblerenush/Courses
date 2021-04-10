#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

std::vector<std::string> SplitIntoWords(const std::string& s)
{
  std::vector<std::string> results;

  auto begin_iter = s.begin();
  while (begin_iter != s.end())
  {
    auto end_iter = std::find(begin_iter, s.end(), ' ');
    results.push_back(std::string{ begin_iter, end_iter });
    if (end_iter != s.end())
    {
      begin_iter = end_iter + 1;
    }
    else
    {
      begin_iter = end_iter;
    }
  }

  return results;
}

int main() {
  string s = "C Cpp Java Python";

  vector<string> words = SplitIntoWords(s);
  cout << words.size() << " ";
  for (auto it = begin(words); it != end(words); ++it) {
    if (it != begin(words)) {
      cout << "/";
    }
    cout << *it;
  }
  cout << endl;

  return 0;
}
