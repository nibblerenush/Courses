#include <iostream>
#include <set>

int main()
{
  int N;
  std::cin >> N;

  std::set<std::string> strs;
  for (int counter = 0; counter < N; ++counter)
  {
    std::string str;
    std::cin >> str;
    strs.insert(str);
  }

  std::cout << strs.size() << std::endl;
  return 0;
}
