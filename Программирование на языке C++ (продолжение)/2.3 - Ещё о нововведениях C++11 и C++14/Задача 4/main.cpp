#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

template<class F>
int* find_if(int* p, int* q, F f)
{
  for (; p != q; ++p)
    if (f(*p))
      return p;
  return q;
}

int main() {
  int primes[5] = { 2,3,5,7,11 };

  int m[10] = { 0,0,1,1,4,6,7,8,9,10 };

  auto gen_finder = [](int* p, int* q) {
    return[p, q](int num) {
      for (int* t = p; t != q; ++t) {
        if (*t == num) {
          return true;
        }
      }
      return false;
    };
  };

  // first_prime будет указывать на число 7
  int* first_prime = find_if(m, m + 10, gen_finder(primes, primes + 5));

  return 0;
}
