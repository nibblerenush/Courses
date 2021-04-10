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

char* resize(const char* str, unsigned size, unsigned new_size) {
  char* new_str = new char[new_size];

  if (new_size <= size) {
    copy(str, str + new_size, new_str);
  }
  else {
    copy(str, str + size, new_str);
  }

  delete[] str;
  return new_str;
}

int main() {
  return 0;
}
