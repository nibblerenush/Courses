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
void for_each_int(int* p, int* q, F f)
{
  for (; p != q; ++p)
    f(*p);
}

int main() {
  int m[10] = { 1,2,3,4,5,6,7,8,9,10 };

  auto square_fun = [](int& num) { num *= num; };
  
  for_each_int(m, m + 10, square_fun); // теперь m = {1,4,9,16,25,36,49,64,81,100};

  return 0;
}
