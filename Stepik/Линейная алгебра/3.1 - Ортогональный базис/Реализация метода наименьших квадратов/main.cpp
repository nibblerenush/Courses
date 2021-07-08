#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

int ScalarProduct(const vector<double>& lvector, const vector<double>& rvector) {
  if (lvector.size() != rvector.size()) {
    throw invalid_argument("ScalarProduct: not equal sizes");
  }

  double scalar_product = 0.0;
  for (int i = 0; i < (int)lvector.size(); ++i) {
    scalar_product += lvector[i] * rvector[i];
  }

  return scalar_product;
}

bool isZero(double num) {
  return abs(num) < 0.000001;
}

bool isZeroRow(const vector<double>& row) {
  for (double num : row) {
    if (!isZero(num)) {
      return false;
    }
  }

  return true;
}

class EquationMatrix {
public:
  EquationMatrix(int equations_count, int unknowns_count) :
    m_rows_count(unknowns_count), m_cols_count(unknowns_count + 1)
  {
    auto check_input_argument =
      [](int input_argument, const string& message)
    {
      if (input_argument < 1) {
        throw invalid_argument(message);
      }
    };

    check_input_argument(equations_count, "Invalid equations_count");
    check_input_argument(unknowns_count, "Invalid unknowns_count");

    if (equations_count < unknowns_count) {
      throw invalid_argument("Value of equations_count less than value of unknowns_count");
    }

    // basis vectors
    vector<vector<double>> e(unknowns_count, vector<double>(equations_count, 0.0));

    // vector f
    vector<double> f(equations_count, 0.0);

    for (int i = 0; i < equations_count; ++i) {
      for (int j = 0; j < unknowns_count; ++j) {
        cin >> e[j][i];
      }
      cin >> f[i];
    }

    m_matrix.assign(m_rows_count, vector<double>(m_cols_count));
    
    for (int i = 0; i < m_rows_count; ++i) {
      m_matrix[i][m_cols_count - 1] = ScalarProduct(f, e[i]);

      for (int j = 0; j < m_cols_count - 1; ++j) {
        m_matrix[i][j] = ScalarProduct(e[i], e[j]);
      }
    }
  }

  void Print() const {
    Print(m_cols_count - 1);
  }

  void PrintFull() const {
    Print(m_cols_count);
  }

  void DirectPath() {
    for (int k = 0; k < min(m_rows_count, m_cols_count - 1); ++k) {
      int idx_max = k;
      double max = m_matrix[idx_max][k];

      for (int i = k + 1; i < m_rows_count; ++i) {
        if (abs(m_matrix[i][k]) > abs(max)) {
          max = m_matrix[i][k];
          idx_max = i;
        }
      }

      if (isZero(m_matrix[idx_max][k])) {
        continue;
      }

      if (idx_max != k) {
        swap(m_matrix[k], m_matrix[idx_max]);
      }

      for (int i = k + 1; i < m_rows_count; ++i) {
        double coef = m_matrix[i][k] / m_matrix[k][k];

        for (int j = k + 1; j < m_cols_count; ++j) {
          m_matrix[i][j] -= m_matrix[k][j] * coef;
        }

        m_matrix[i][k] = 0;
      }
    }
  }

  vector<double> ReversePath() const {
    vector<double> solution(m_cols_count - 1);

    int last_non_zero_row = m_rows_count - 1;
    while (last_non_zero_row >= 0 &&
      isZeroRow(m_matrix[last_non_zero_row]))
    {
      last_non_zero_row -= 1;
    }

    for (int i = last_non_zero_row; i >= 0; --i) {
      solution[i] = m_matrix[i][m_cols_count - 1];

      for (int j = i + 1; j <= last_non_zero_row; ++j) {
        solution[i] -= m_matrix[i][j] * solution[j];
      }

      solution[i] /= m_matrix[i][i];
    }

    return solution;
  }

  void ComputeRank() {
    m_rank = ComputeRank(m_cols_count - 1);
    m_full_rank = ComputeRank(m_cols_count);
  }

  int GetRank() const {
    return m_rank;
  }

  int GetFullRank() const {
    return m_full_rank;
  }

private:
  int ComputeRank(int size) const {
    int rank = 0;

    for (const auto& row : m_matrix) {
      for (int i = 0; i < size; ++i) {
        if (!isZero(row[i])) {
          rank += 1;
          break;
        }
      }
    }

    return rank;
  }

  void Print(int size) const {
    static const int WIDTH = 10;

    cout << "{\n";
    for (const auto& row : m_matrix) {
      for (int i = 0; i < size; ++i) {
        cout << setw(WIDTH) << row[i] << ' ';
      }
      cout << '\n';
    }
    cout << "}\n";
  }

private:
  const int m_rows_count;
  const int m_cols_count;
  vector<vector<double>> m_matrix;

  int m_rank = 0;
  int m_full_rank = 0;
};

int main() {
  int equations_count = 0;
  int unknowns_count = 0;
  cin >> equations_count >> unknowns_count;
  EquationMatrix em(equations_count, unknowns_count);

  //em.PrintFull();

  em.DirectPath();

  auto solution = em.ReversePath();
  for (double num : solution) {
    cout << setprecision(10) << (isZero(num) ? 0. : num) << ' ';
  }

  return 0;
}

/*
4 2
4 2 8
5 2 4
2 6 2
3 0 8
*/

/*
3 2
1 -1 4
1 0 5
1 1 9
*/
