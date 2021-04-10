#include <algorithm>
#include <array>
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

template<class Iterator>
size_t count_permutations(Iterator p, Iterator q) {
  size_t result = 0;
  std::vector<typename std::iterator_traits<Iterator>::value_type> arr(p, q);
  std::sort(arr.begin(), arr.end());

  do {
    if (std::adjacent_find(arr.begin(), arr.end()) == arr.end()) {
      result += 1;
    }
    
  } while (std::next_permutation(arr.begin(), arr.end()));

  return result;
}

int main() {
  std::array<int, 3> a1 = { 1,2,3 };
  size_t c1 = count_permutations(a1.cbegin(), a1.cend()); // 6

  std::array<int, 5> a2 = { 1,2,3,4,4 };
  size_t c2 = count_permutations(a2.cbegin(), a2.cend()); // 36

  return 0;
}
