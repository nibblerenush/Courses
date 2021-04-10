#include <iostream>
#include <string>

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

int main()
{
  std::string str;
  std::cin >> str;
  std::cout << std::boolalpha << IsPalindrom(str);
  return 0;
}
