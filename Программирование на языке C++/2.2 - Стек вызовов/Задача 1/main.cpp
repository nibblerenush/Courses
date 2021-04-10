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

unsigned gcd(unsigned a, unsigned b) {
  if (a == 0) {
    return b;
  }
  
  if (b == 0) {
    return a;
  }
  
  if (a >= b) {
    return gcd(a % b, b);
  }

  if (b > a) {
    return gcd(a, b % a);
  }
}

int main() {
  return 0;
}
