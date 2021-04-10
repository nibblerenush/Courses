#include <cassert>
#include <iostream>
#include <vector>

class Fibonacci {
public:
  static int get_remainder(long long n, int m) {
    assert(n >= 1);
    assert(m >= 2);
    
    std::vector<int> pisano(6 * m, 0);
    pisano[0] = 0 % m;
    pisano[1] = 1 % m;
    int pissano_length = 2;
    
    for (int i = 2; i < pisano.size(); ++i) {
      pisano[i] = (pisano[i - 1] + pisano[i - 2]) % m;
      if (pisano[i] == 1 && pisano[i - 1] == 0) {
        break;
      }
      else {
        pissano_length++;
      }
    }
    
    if (pissano_length != pisano.size()) {
      pissano_length--;
    }

    return pisano[n % pissano_length];
  }
};

int main(void) {
  long long n;
  int m;
  std::cin >> n >> m;
  std::cout << Fibonacci::get_remainder(n, m) << std::endl;
  return 0;
}
