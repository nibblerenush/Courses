#include <iostream>
#include <vector>

int main()
{
  int n;
  std::cin >> n;
  
  std::vector<int> result;
  
  // Main algorithm
  int number = 0;
  while (true)
  {
    number++;
    n -= number;

    if (n > number) {
      result.push_back(number);
    }
    else {
      result.push_back(number + n);
      break;
    }
  }

  std::cout << result.size() << std::endl;
  for (const auto& number : result) {
    std::cout << number << ' ';
  }

  return 0;
}
