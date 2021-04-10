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

int** transpose(const int* const* m, unsigned rows, unsigned cols) {
  int** new_matrix = new int* [cols];
  for (unsigned i = 0; i < cols; ++i) {
    new_matrix[i] = new int[rows];
  }

  for (unsigned i = 0; i < rows; ++i) {
    for (unsigned j = 0; j < cols; ++j) {
      new_matrix[j][i] = m[i][j];
    }
  }

  return new_matrix;
}

int main() {
  return 0;
}
