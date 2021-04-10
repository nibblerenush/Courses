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

void swap_min(int* m[], unsigned rows, unsigned cols) {
  int min_elem = m[0][0];
  int min_row = 0;

  for (unsigned i = 0; i < rows; ++i) {
    for (unsigned j = 0; j < cols; ++j) {
      if (m[i][j] < min_elem) {
        min_elem = m[i][j];
        min_row = i;
      }
    }
  }

  int* temp = m[0];
  m[0] = m[min_row];
  m[min_row] = temp;
}

int main() {
  return 0;
}
