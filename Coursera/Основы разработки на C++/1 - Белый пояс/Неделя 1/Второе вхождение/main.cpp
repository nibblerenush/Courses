#include <iostream>
#include <string>

int main()
{
  std::string str;
  std::cin >> str;

  int index;
  int count = 0;

  for (std::size_t i = 0; i < str.length(); ++i)
  {
    if (str[i] == 'f')
    {
      count++;
    }

    if (count == 2)
    {
      index = i;
      break;
    }
  }

  if (count == 0)
  {
    std::cout << -2;
  }
  else if (count == 1)
  {
    std::cout << -1;
  }
  else
  {
    std::cout << index;
  }

  return 0;
}
