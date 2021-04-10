#include <iostream>

int get_change(int m) {
  //write your code here
  int result = 0;
  while (m != 0) {
    if (m - 10 >= 0) {
      m -= 10;
    }
    else if (m - 5 >= 0) {
      m -= 5;
    }
    else {
      m -= 1;
    }
    result += 1;
  }
  
  return result;
}

int main() {
  int m;
  std::cin >> m;
  std::cout << get_change(m) << '\n';
}
