#include <iostream>
#include <string>
#include <vector>

bool IsPalindrom(std::string str)
{
  if (str.length() == 0 || str.length() == 1)
  {
    return true;
  }

  int head = 0;
  int tail = str.length() - 1;

  while (head < tail)
  {
    if (str[head++] != str[tail--])
    {
      return false;
    }
  }

  return true;
}

std::vector<std::string> PalindromFilter(std::vector<std::string> words, int minLength)
{
  std::vector<std::string> result;
  for (auto word : words)
  {
    if (IsPalindrom(word) && word.length() >= minLength)
    {
      result.push_back(word);
    }
  }
  return result;
}

int main()
{
  std::vector<std::string> words(5);
  for (auto & word : words)
  {
    std::cin >> word;
  }

  int minLength;
  std::cin >> minLength;
  auto results = PalindromFilter(words, minLength);

  for (auto result : results)
  {
    std::cout << result << ' ';
  }

  return 0;
}
