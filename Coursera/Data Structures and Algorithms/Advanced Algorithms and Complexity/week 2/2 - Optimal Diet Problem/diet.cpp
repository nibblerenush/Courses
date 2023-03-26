#include <algorithm>
#include <cstdio>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

const double EPS = 1e-3;
const double INF = 1e9;

typedef std::vector<double> Column;
typedef std::vector<double> Row;
typedef std::vector<Row> Matrix;

bool IsEqual(double a, double b) {
  return std::abs(a - b) <= EPS;
}

struct Equation {
  Equation(const Matrix& a, const Column& b) : a(a), b(b)
  {}

  Matrix a;
  Column b;
};

struct Position {
  Position(int column, int row) : column(column), row(row)
  {}

  int column;
  int row;
};

Position SelectPivotElement(
  const Matrix& a,
  std::vector <bool>& used_rows,
  std::vector <bool>& used_columns) {

  Position pivot_element(0, 0);
  const int size = a.size();
  
  while (pivot_element.row < size && used_rows[pivot_element.row]) {
    ++pivot_element.row;
  }
    
  while (pivot_element.column < size && used_columns[pivot_element.column]) {
    ++pivot_element.column;
  }

  while (pivot_element.row < size && IsEqual(a[pivot_element.row][pivot_element.column], 0)) {
    ++pivot_element.row;
  }

  if (pivot_element.row >= size) {
    pivot_element.row = size - 1;
  }
  
  return pivot_element;
}

void SwapLines(Matrix& a, Column& b, std::vector <bool>& used_rows, Position& pivot_element) {
  std::swap(a[pivot_element.column], a[pivot_element.row]);
  std::swap(b[pivot_element.column], b[pivot_element.row]);
  used_rows.swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
  pivot_element.row = pivot_element.column;
}

void ProcessPivotElement(Matrix& a, Column& b, const Position& pivot_element) {
  const double pivot_value = a[pivot_element.row][pivot_element.column];

  // skip
  if (IsEqual(pivot_value, 0)) {
    return;
  }
  
  for (double& elem : a[pivot_element.row]) {
    elem /= pivot_value;
  }
  b[pivot_element.row] /= pivot_value;

  const int size = a.size();
  for (int i = 0; i < size; ++i) {
    if (i != pivot_element.row) {
      const double coeff = a[i][pivot_element.column];
      for (int j = 0; j < size; ++j) {
        a[i][j] -= coeff * a[pivot_element.row][j];
      }
      b[i] -= coeff * b[pivot_element.row];
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

class BinomialCoefficient {
public:
  BinomialCoefficient(int k, int n) : m_k(k), m_n(n) {
    if (k < 0 || k > n) {
      throw std::invalid_argument("Invalid argument k");
    }

    if (n < 0) {
      throw std::invalid_argument("Invalid argument n");
    }
  }

  std::vector<std::vector<int>> GetChoices() const {
    std::vector<int> choice;
    std::vector<std::vector<int>> choices;
    backtrack(0, 0, choice, choices);
    return choices;
  }

private:
  void backtrack(
    int begin_index,
    int depth,
    std::vector<int>& choice,
    std::vector<std::vector<int>>& choices) const
  {
    if ((int)choice.size() == m_k) {
      choices.push_back(choice);
      return;
    }

    for (int i = begin_index; i <= m_n - m_k + depth; ++i) {
      choice.push_back(i);
      backtrack(i + 1, depth + 1, choice, choices);
      choice.pop_back();
    }
  }

private:
  const int m_k;
  const int m_n;
};

bool checkEquationSolution(
  const Matrix& A,
  const Column& b,
  const Column& equationSolution)
{
  const int n = A.size();
  const int m = equationSolution.size();
  
  for (int i = 0; i < n; ++i) {
    double leftPart = 0;
    for (int j = 0; j < m; ++j) {
      leftPart += A[i][j] * equationSolution[j];
    }
    
    if (leftPart > b[i] + EPS) {
      return false;
    }
  }
  
  return true;
}

std::pair<int, std::vector<double>> solve_diet_problem(
  int n,
  int m,
  Matrix A,
  std::vector<double> b,
  std::vector<double> c)
{
  int answerType = -1;
  std::vector<double> answerAmounts(m, 0);
  double maxPleasure = std::numeric_limits<double>::lowest();

  BinomialCoefficient bc(m, n + m + 1);
  const auto subsets = bc.GetChoices();

  // Add inequalities amount[i] >= 0 (or -amount[i] <= 0)
  for (int i = 0; i < m; ++i) {
    Row row(m, 0);
    row[i] = -1;
    A.push_back(std::move(row));
    b.push_back(0);
  }

  // Add inequality amount[0] + ... + amount[m - 1] <= 10^9
  A.push_back(Row(m, 1));
  b.push_back(INF);

  for (const auto& subset : subsets) {
    Matrix matrix;
    Column column;

    for (int index : subset) {
      matrix.push_back(A[index]);
      column.push_back(b[index]);
    }

    Equation equation(matrix, column);
    auto equationSolution = SolveEquation(equation);

    if (checkEquationSolution(A, b, equationSolution)) {
      double pleasure = 0;
      for (int i = 0; i < m; ++i) {
        pleasure += equationSolution[i] * c[i];
      }

      if (pleasure > maxPleasure) {
        maxPleasure = pleasure;
        answerType = 0;
        answerAmounts = move(equationSolution);
      }
    }
  }
  
  if (answerType == 0) {
    double amountSum = std::accumulate(answerAmounts.begin(), answerAmounts.end(), 0.0);
    if (amountSum + EPS > INF) {
      answerType = 1;
    }
  }

  return { answerType, answerAmounts };
}

int main() {
  int n, m;
  std::cin >> n >> m;
  Matrix A(n, std::vector<double>(m));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cin >> A[i][j];
    }
  }
  std::vector<double> b(n);
  for (int i = 0; i < n; i++) {
    std::cin >> b[i];
  }
  std::vector<double> c(m);
  for (int i = 0; i < m; i++) {
    std::cin >> c[i];
  }

  std::pair<int, std::vector<double>> ans = solve_diet_problem(n, m, A, b, c);

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
