#include <ios>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

struct ConvertILPToSat {
  vector< vector<int> > A;
  vector<int> b;

  ConvertILPToSat(int n, int m) : A(n, vector<int>(m)), b(n)
  {}

  vector<vector<int>> getCombinations(int num) {
    if (num == 1) {
      return { {0}, {1} };
    }
    else if (num == 2) {
      return { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
    }
    else {
      return { {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1} };
    }
  }

  void printEquisatisfiableSatFormula() {
    // This solution prints a simple satisfiable formula
    // and passes about half of the tests.
    // Change this function to solve the problem.

    const int n = A.size();
    const int m = A[0].size();

    int clauseCount = 0;
    int variableCount = m;

    for (int i = 0; i < n; ++i) {
      const auto& row = A[i];
      
      int nonZeroCount = 0;
      for (int coeff : row) {
        if (coeff != 0) {
          ++nonZeroCount;
        }
      }

      if (nonZeroCount == 0) {
        if (0 <= b[i]) {
          continue;
        }
        else {
          cout << "2 1\n";
          cout << "1 0\n";
          cout << "-1 0\n";
          return;
        }
      }

      vector<vector<int>> combinations = getCombinations(nonZeroCount);

      for (const auto& combination : combinations) {
        int combIdx = 0;
        int leftSum = 0;
        for (int coeff : row) {
          if (coeff != 0) {
            leftSum += coeff * combination[combIdx];
            ++combIdx;
          }
        }

        if (leftSum <= b[i]) {
          continue;
        }

        combIdx = 0;
        for (int j = 0; j < m; ++j) {
          if (row[j] != 0) {
            if (combination[combIdx]) {
              oss << -(j + 1) << ' ';
            }
            else {
              oss << (j + 1) << ' ';
            }

            ++combIdx;
          }
        }

        ++clauseCount;
        oss << "0 \n";
      }
    }

    if (clauseCount == 0) {
      cout << "1 1\n";
      cout << "1 -1 0\n";
      return;
    }
    
    cout << clauseCount << ' ' << variableCount << '\n';
    cout << oss.str();
  }

private:
  ostringstream oss;
};

int main() {
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  ConvertILPToSat converter(n, m);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> converter.A[i][j];
    }
  }
  for (int i = 0; i < n; i++) {
    cin >> converter.b[i];
  }

  converter.printEquisatisfiableSatFormula();

  return 0;
}
