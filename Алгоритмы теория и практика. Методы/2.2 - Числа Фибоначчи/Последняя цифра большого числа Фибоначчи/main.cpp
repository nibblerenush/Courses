#include <cassert>
#include <iostream>
#include <vector>

class Fibonacci {
public:
  static int get_last_digit(int n) {
    assert(n >= 1);
    
    std::vector<int> result(n + 1);
    result[0] = 0;
    result[1] = 1;
    for (std::size_t i = 2; i < result.size(); ++i) {
      result[i] = (result[i - 1] + result[i - 2]) % 10;
    }
    n = result[n];

    return n;
  }
};

int main(void) {
  int n;
  std::cin >> n;
  std::cout << Fibonacci::get_last_digit(n) << std::endl;
  return 0;
}
