#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SortedStrings
{
public:
  void AddString(const std::string& s)
  {
    strs.push_back(s);
  }
  std::vector<std::string> GetSortedStrings()
  {
    std::sort(std::begin(strs), std::end(strs));
    return strs;
  }
private:
  std::vector<std::string> strs;
};

void PrintSortedStrings(SortedStrings& strings) {
  for (const string& s : strings.GetSortedStrings()) {
    cout << s << " ";
  }
  cout << endl;
}

int main() {
  SortedStrings strings;

  strings.AddString("first");
  strings.AddString("third");
  strings.AddString("second");
  PrintSortedStrings(strings);

  strings.AddString("second");
  PrintSortedStrings(strings);

  return 0;
}
