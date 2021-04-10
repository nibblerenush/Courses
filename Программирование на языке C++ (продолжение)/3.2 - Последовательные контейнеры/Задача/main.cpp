#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <list>
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

template<class It>
size_t max_increasing_len(It p, It q) {
  int result = 0;
  if (p == q) {
    return result;
  }
  
  ++p;
  int count = 1;
  result = std::max(result, count);
  
  while (p != q) {
    if (*p > *std::prev(p)) {
      count += 1;
    }
    else {
      result = std::max(result, count);
      count = 1;
    }
    ++p;
  }

  result = std::max(result, count);
  return result;
}

int main() {
  std::list<int> const l1 = { 7,8,9,4,5,6,1,2,3,4 };
  size_t len1 = max_increasing_len(l1.begin(), l1.end()); // 4, соответствует подотрезку 1,2,3,4

  std::list<int> const l2 = { -3,-2,-1,0,0,1,2,3,4,5 };
  size_t len2 = max_increasing_len(l2.begin(), l2.end()); // 6, соответствует подотрезку 0,1,2,3,4,5

  return 0;
}
