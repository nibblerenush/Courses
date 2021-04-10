#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

typedef vector<vector<double>> matrix;

// ===== Gaussian elimination =====
typedef std::vector<double> Column;
typedef std::vector<double> Row;
typedef std::vector<Row> Matrix;

struct Equation {
  Equation(const Matrix& a, const Column& b) :
    a(a),
    b(b)
  {}

  Matrix a;
  Column b;
};

struct Position {
  Position(int column, int row) :
    column(column),
    row(row)
  {}

  int column;
  int row;
};

Position SelectPivotElement(
  const Matrix& a,
  std::vector <bool>& used_rows,
  std::vector <bool>& used_columns) {
  // This algorithm selects the first free element.
  // You'll need to improve it to pass the problem.
  Position pivot_element(0, 0);
  while (used_rows[pivot_element.row])
    ++pivot_element.row;
  while (used_columns[pivot_element.column])
    ++pivot_element.column;

  while (pivot_element.row < (int)a.size() && a[pivot_element.row][pivot_element.column] == 0) {
    pivot_element.row += 1;
  }

  if (pivot_element.row >= (int)a.size()) {
    pivot_element.row = (int)a.size() - 1;
  }

  return pivot_element;
}

void SwapLines(Matrix& a, Column& b, std::vector <bool>& used_rows, Position& pivot_element) {
  swap(a[pivot_element.column], a[pivot_element.row]);
  swap(b[pivot_element.column], b[pivot_element.row]);
  swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
  pivot_element.row = pivot_element.column;
}

void ProcessPivotElement(Matrix& a, Column& b, const Position& pivot_element) {
  // Write your code here
  double pe_val = a[pivot_element.row][pivot_element.column];

  // skip
  if (pe_val == 0) {
    return;
  }

  // Rescale to make pivot 1
  Row& row = a[pivot_element.row];
  for (double& val : row) {
    val /= pe_val;
  }
  b[pivot_element.row] /= pe_val;

  // Subtract row from others to make
  // other entries in column 0
  for (int i = 0; i < (int)a.size(); ++i) {
    if (i != pivot_element.row) {
      Row copy_row = row;
      double entry = a[i][pivot_element.column];

      for (double& val : copy_row) {
        val *= entry;
      }
      double copy_b = b[pivot_element.row] * entry;

      for (int j = 0; j < (int)a.size(); ++j) {
        a[i][j] -= copy_row[j];
      }
      b[i] -= copy_b;
    }
  }
}

void MarkPivotElementUsed(const Position& pivot_element, std::vector <bool>& used_rows, std::vector <bool>& used_columns) {
  used_rows[pivot_element.row] = true;
  used_columns[pivot_element.column] = true;
}

Column SolveEquation(Equation equation) {
  Matrix& a = equation.a;
  Column& b = equation.b;
  int size = a.size();

  std::vector <bool> used_columns(size, false);
  std::vector <bool> used_rows(size, false);
  for (int step = 0; step < size; ++step) {
    Position pivot_element = SelectPivotElement(a, used_rows, used_columns);
    SwapLines(a, b, used_rows, pivot_element);
    ProcessPivotElement(a, b, pivot_element);
    MarkPivotElementUsed(pivot_element, used_rows, used_columns);
  }

  return b;
}
// ===== Gaussian elimination =====

// ===== GetSubsets =====
class GetSubsets {
public:
  vector<vector<int>> subsets(vector<int>& nums) {
    vector<int> subset;
    vector<vector<int>> result;
    backtrack(nums, 0, subset, result);
    return result;
  }

private:
  void backtrack(const vector<int>& nums, int index, vector<int>& subset, vector<vector<int>>& subsets) {
    subsets.push_back(subset);

    for (int i = index; i < (int)nums.size(); ++i) {
      subset.push_back(nums[i]);
      backtrack(nums, i + 1, subset, subsets);
      subset.pop_back();
    }
  }
};
// ===== GetSubsets =====

bool check_equation_solution(const Column& solve_equation, const matrix& A, const vector<double>& b) {
  // Check besides last
  for (int i = 0; i < (int)A.size() - 1; ++i) {
    double left_side = 0;
    for (int j = 0; j < (int)A[0].size(); ++j) {
      left_side += A[i][j] * solve_equation[j];
    }
    
    // Check is equal
    if (std::fabs(left_side - b[i]) < 1e-3) {
      continue;
    }
    
    // If greater
    if (left_side > b[i]) {
      return false;
    }
  }
  
  return true;
}

pair<int, vector<double>> solve_diet_problem(
    int n, 
    int m, 
    matrix A, 
    vector<double> b, 
    vector<double> c) {

  // Write your code here
  vector<int> initial_set(n + m + 1);
  iota(initial_set.begin(), initial_set.end(), 0);
  
  GetSubsets get_subsets;
  auto subsets = get_subsets.subsets(initial_set);

  // Add amount[i] >= 0
  for (int i = 0; i < m; ++i) {
    vector<double> row(m, 0);
    row[i] = -1;
    A.push_back(move(row));
    b.push_back(0);
  }

  // Add amount[1] + amount[2] + ... + amount[m] <= 10^9
  A.push_back(vector<double>(m, 1));
  b.push_back((double)1'000'000'000);

  int ans_type = -1;
  vector<double> ans_amounts(m, 0);
  double max_pleasure = std::numeric_limits<double>::lowest();

  for (int i = 0; i < (int)subsets.size(); ++i) {
    // Only m inequations
    if (subsets[i].size() == m) {
      Matrix cur_a;
      Column cur_b;

      for (int num : subsets[i]) {
        cur_a.push_back(A[num]);
        cur_b.push_back(b[num]);
      }

      Equation equation(cur_a, cur_b);
      auto solve_equation = SolveEquation(equation);

      if (check_equation_solution(solve_equation, A, b)) {
        // Compute pleasure
        double pleasure = 0;
        for (int j = 0; j < m; ++j) {
          pleasure += solve_equation[j] * c[j];
        }

        if (pleasure > max_pleasure) {
          max_pleasure = pleasure;
          ans_amounts = solve_equation;
          ans_type = 0;
        }
      }
    }
  }

  if (ans_type == 0) {
    double amount_sum = std::accumulate(ans_amounts.begin(), ans_amounts.end(), 0.0);
    if (amount_sum > (double)999'999'999) {
      ans_type = 1;
    }
  }

  return {ans_type, ans_amounts};
}

int main(){
  int n, m;
  cin >> n >> m;
  matrix A(n, vector<double>(m));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> A[i][j];
    }
  }
  vector<double> b(n);
  for (int i = 0; i < n; i++) {
    cin >> b[i];
  }
  vector<double> c(m);
  for (int i = 0; i < m; i++) {
    cin >> c[i];
  }

  pair<int, vector<double>> ans = solve_diet_problem(n, m, A, b, c);

  switch (ans.first) {
    case -1: 
      printf("No solution\n");
      break;
    case 0: 
      printf("Bounded solution\n");
      for (int i = 0; i < m; i++) {
        printf("%.18f%c", ans.second[i], " \n"[i + 1 == m]);
      }
      break;
    case 1:
      printf("Infinity\n");
      break;      
  }
  return 0;
}
