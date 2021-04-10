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

template <typename T, typename U>
void copy_n_(T* dst, U* src, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    dst[i] = static_cast<T>(src[i]);
  }
}

int main() {
  int ints[] = { 1, 2, 3, 4 };
  double doubles[4] = {};
  copy_n_(doubles, ints, 4); // теперь в массиве doubles содержатся элементы 1.0, 2.0, 3.0 и 4.0
  return 0;
}
