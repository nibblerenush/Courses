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


// алгоритм должен работать с forward итераторами
// и возвращать итератор, который потом будет передан
// в метод erase соответствующего контейнера
template<class FwdIt>
FwdIt remove_nth_(FwdIt p, FwdIt q, size_t n) {
  if (p == q) {
    return p;
  }
  
  for (size_t i = 0; i < n; ++i) {
    ++p;
    if (p == q) {
      return p;
    }
  }

  FwdIt p2 = std::next(p);
  while (p2 != q) {
    *p++ = *p2++;
  }

  return p;
}

int main() {
  std::vector<int> v = { 0,1,2,3,4,5,6,7,8,9,10 };
  v.erase(remove_nth_(v.begin(), v.end(), 25), v.end());
  // теперь в v = {0,1,2,3,4,6,7,8,9,10};

  return 0;
}
