#include <algorithm>
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

int two_to_p(int a) {
  int result = 0;
  int A = 1;

  while (A <= a) {
    result += 1;  
    A <<= 1;
  }

  return result - 1;
}

int main() {
  int T = 0;
  cin >> T;

  for (int i = 0; i < T; ++i) {
    int a = 0;
    cin >> a;
    cout << two_to_p(a) << endl;
  }
  
  return 0;
}
