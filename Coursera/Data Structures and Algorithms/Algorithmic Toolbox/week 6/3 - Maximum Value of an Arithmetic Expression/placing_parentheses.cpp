#include <algorithm>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::max;
using std::min;

long long eval(long long a, long long b, char op) {
  if (op == '*') {
    return a * b;
  } else if (op == '+') {
    return a + b;
  } else if (op == '-') {
    return a - b;
  } else {
    assert(0);
  }
}

std::pair<long long, long long> min_and_max(
  int i,
  int j,
  const vector<vector<long long>>& M,
  const vector<vector<long long>>& m,
  const vector<char>& operations)
{
  long long min_result = std::numeric_limits<long long>::max();
  long long max_result = std::numeric_limits<long long>::min();

  for (int k = i; k < j; ++k) {
    long long a = eval(M[i][k], M[k + 1][j], operations[k]);
    long long b = eval(M[i][k], m[k + 1][j], operations[k]);
    long long c = eval(m[i][k], M[k + 1][j], operations[k]);
    long long d = eval(m[i][k], m[k + 1][j], operations[k]);
    min_result = min({ min_result, a, b, c, d });
    max_result = max({ max_result, a, b, c, d });
  }

  return { min_result, max_result };
}

long long get_maximum_value(const string &exp) {
  vector<int> digits;
  vector<char> operations;

  for (char c : exp) {
    if (std::isdigit(c)) {
      digits.push_back(c - '0');
    }
    else {
      operations.push_back(c);
    }
  }

  int n = digits.size();
  vector<vector<long long>> M(n, vector<long long>(n, 0));
  vector<vector<long long>> m(n, vector<long long>(n, 0));

  for (int i = 0; i < n; ++i) {
    M[i][i] = digits[i];
    m[i][i] = digits[i];
  }
  
  for (int s = 1; s <= n - 1; ++s) {
    for (int i = 0; i <= n - s - 1; ++i) {
      int j = i + s;
      std::pair<long long, long long> min_max = min_and_max(i, j, M, m, operations);
      m[i][j] = min_max.first;
      M[i][j] = min_max.second;
    }
  }

  return M[0][n - 1];
}

int main() {
  string s;
  std::cin >> s;
  std::cout << get_maximum_value(s) << '\n';
}
