#include <iostream>
#include <map>
#include <string>

std::map<char, int> BuildCharCounters(const std::string& word)
{
  std::map<char, int> result;
  for (const auto& letter : word)
  {
    ++result[letter];
  }
  return result;
}

int main()
{
  int N;
  std::cin >> N;

  for (int i = 0; i < N; ++i)
  {
    std::string word1;
    std::string word2;
    std::cin >> word1 >> word2;
    if (BuildCharCounters(word1) == BuildCharCounters(word2))
    {
      std::cout << "YES" << std::endl;
    }
    else
    {
      std::cout << "NO" << std::endl;
    }
  }

  return 0;
}
