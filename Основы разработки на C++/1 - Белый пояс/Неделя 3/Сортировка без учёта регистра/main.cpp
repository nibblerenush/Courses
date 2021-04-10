#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

int main()
{
  int N;
  std::cin >> N;

  std::vector<std::string> strs(N);
  for (auto& str : strs)
  {
    std::cin >> str;
  }

  std::sort(std::begin(strs), std::end(strs), [](std::string str1, std::string str2)
  {
    auto tolower_string = [](char& symbol) { symbol = std::tolower(symbol); };
    std::for_each(std::begin(str1), std::end(str1), tolower_string);
    std::for_each(std::begin(str2), std::end(str2), tolower_string);
    return str1 < str2;
  });

  for (const auto& str : strs)
  {
    std::cout << str << ' ';
  }
  std::cout << std::endl;

  return 0;
}
