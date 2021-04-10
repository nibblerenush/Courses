#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

template<typename Key, typename Value>
Value& GetRefStrict(std::map<Key, Value>& dict, const Key& key)
{
  if (dict.count(key))
  {
    return dict.at(key);
  }
  else
  {
    throw std::runtime_error("!dict.count(key)");
  }
}

int main()
{
  std::map<int, std::string> m = { {0, "value"} };
  std::string& item = GetRefStrict(m, 0);
  item = "newvalue";
  std::cout << m[0] << std::endl;
  return 0;
}
