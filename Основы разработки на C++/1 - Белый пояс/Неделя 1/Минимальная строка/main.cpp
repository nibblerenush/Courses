#include <iostream>
#include <string>

int main()
{
  std::string str1;
  std::string str2;
  std::string str3;

  std::cin >> str1 >> str2 >> str3;

  if (str1 < str2)
  {
    if (str1 < str3)
    {
      std::cout << str1;
    }
    else
    {
      std::cout << str3;
    }
  }
  else
  {
    if (str2 < str3)
    {
      std::cout << str2;
    }
    else
    {
      std::cout << str3;
    }
  }

  return 0;
}
