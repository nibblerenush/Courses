#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

bool isZero(double num) {
  return abs(num) < 0.000001;
}

class SquareMatrix {
public:
  SquareMatrix(int matrix_size) : m_matrix_size(matrix_size) {
    if (m_matrix_size < 1) {
      throw invalid_argument("Invalid matrix_size");
    }
    
    m_matrix.assign(m_matrix_size, vector<double>(m_matrix_size));
    
    for (int i = 0; i < m_matrix_size; ++i) {
      for (int j = 0; j < m_matrix_size; ++j) {
        cin >> m_matrix[i][j];
      }
    }
  }
  
  void Print() const {
    static const int WIDTH = 10;
    
    cout << "{\n";
    for (const auto& row : m_matrix) {
      for (double num : row) {
        cout << setw(WIDTH) << num << ' ';
      }
      cout << '\n';
    }
    cout << "}\n";
  }

  void DirectPath() {
    int sign = 1;
    
    for (int k = 0; k < m_matrix_size; ++k) {
      int idx_max = k;
      double max = m_matrix[idx_max][k];

      for (int i = k + 1; i < m_matrix_size; ++i) {
        if (abs(m_matrix[i][k]) > abs(max)) {
          max = m_matrix[i][k];
          idx_max = i;
        }
      }
      
      if (isZero(m_matrix[idx_max][k])) {
        m_determinant = 0.0;
        return;
      }

      if (idx_max != k) {
        swap(m_matrix[k], m_matrix[idx_max]);
        sign *= -1;
      }

      for (int i = k + 1; i < m_matrix_size; ++i) {
        double coef = m_matrix[i][k] / m_matrix[k][k];

        for (int j = k + 1; j < m_matrix_size; ++j) {
          m_matrix[i][j] -= m_matrix[k][j] * coef;
        }

        m_matrix[i][k] = 0;
      }
    }

    m_determinant = 1.0;
    for (int i = 0; i < m_matrix_size; ++i) {
      m_determinant *= m_matrix[i][i];
    }
    m_determinant *= sign;
  }

  double GetDeterminant() const {
    return m_determinant;
  }

private:
  const int m_matrix_size;
  vector<vector<double>> m_matrix;
  double m_determinant = 0.0;
};

int main() {
  int matrix_size = 0;
  cin >> matrix_size;
  SquareMatrix sm(matrix_size);
  
  sm.DirectPath();
  
  double determinant = sm.GetDeterminant();
  cout << setprecision(0) << fixed << round(determinant);
  
  return 0;
}

/*
2
4 2
5 3
*/

/*
4
0 0 3 1
-4 2 4 1
0 2 1 -2
2 1 0 -2
*/

/*
3
3 2 -4
4 1 -2
5 2 -3
*/
