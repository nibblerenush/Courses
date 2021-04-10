#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
  int stairs_count;
  std::cin >> stairs_count;
  
  std::vector<int> stairs(stairs_count + 1);
  for (int i = 1; i <= stairs_count; ++i) {
    std::cin >> stairs[i];
  }
  
  std::vector<int> dynamic(stairs_count + 1);
  dynamic[0] = 0;
  dynamic[1] = stairs[1];
  
  for (int i = 2; i <= stairs_count; ++i) {
    dynamic[i] = std::max(dynamic[i - 1] + stairs[i], dynamic[i - 2] + stairs[i]);
  }
  std::cout << dynamic[stairs_count] << std::endl;

  return 0;
}
