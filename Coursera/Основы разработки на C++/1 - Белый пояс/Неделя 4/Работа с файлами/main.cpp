#include <fstream>
#include <iostream>
#include <string>

void part1(const std::string& input_path)
{
  std::ifstream input(input_path);
  if (input.is_open())
  {
    std::string line;
    while (std::getline(input, line))
    {
      std::cout << line << std::endl;
    }
  }
}

void part2(const std::string& input_path)
{
  std::ifstream input(input_path);
  std::ofstream output("output.txt");
  if (input.is_open())
  {
    std::string line;
    while (std::getline(input, line))
    {
      output << line << std::endl;
    }
  }
}

int main()
{
  const std::string input_path = "input.txt";
  //part1(input_path);
  part2(input_path);
  return 0;
}
