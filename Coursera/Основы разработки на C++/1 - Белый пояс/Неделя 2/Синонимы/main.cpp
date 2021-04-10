#include <iostream>
#include <map>
#include <set>
#include <string>

int main()
{
  const std::string ADD = "ADD";
  const std::string COUNT = "COUNT";
  const std::string CHECK = "CHECK";

  int Q;
  std::cin >> Q;

  std::map<std::string, std::set<std::string>> synonyms;

  for (int counter = 0; counter < Q; ++counter)
  {
    std::string command;
    std::cin >> command;

    if (command == ADD)
    {
      std::string word1, word2;
      std::cin >> word1 >> word2;
      synonyms[word1].insert(word2);
      synonyms[word2].insert(word1);
    }
    else if (command == COUNT)
    {
      std::string word;
      std::cin >> word;
      std::cout << synonyms[word].size() << std::endl;
    }
    else if (command == CHECK)
    {
      std::string word1, word2;
      std::cin >> word1 >> word2;
      if (synonyms.count(word1))
      {
        if (synonyms[word1].count(word2))
        {
          std::cout << "YES" << std::endl;
        }
        else
        {
          std::cout << "NO" << std::endl;
        }
      }
      else
      {
        std::cout << "NO" << std::endl;
      }
    }
  }

  return 0;
}
