#include <iostream>
#include <vector>
#include <string>

void MoveStrings(std::vector<std::string>& source, std::vector<std::string>& destination)
{
  for (auto str : source)
  {
    destination.push_back(str);
  }
  source.clear();
}

int main()
{
  std::vector<std::string> source = { "a", "b", "c" };
  std::vector<std::string> destination = { "z" };
  MoveStrings(source, destination);
  return 0;
}
