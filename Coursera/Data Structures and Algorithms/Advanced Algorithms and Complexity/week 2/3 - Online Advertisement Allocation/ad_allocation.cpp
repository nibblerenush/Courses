/*
 * Оригинальное решение взято отсюда:
 * https://github.com/schostac/Advanced-Algorithms-and-Complexity/blob/master/week2/ad_allocation.cpp
 * Объяснение алгоритма взято отсюда:
 * https://www.youtube.com/watch?v=3waA1pAKT8w
 * https://www.youtube.com/watch?v=E-SuGjUjJ3Y
 * 
 * У меня не получилось самостоятельно решить данную задачу, так как
 * я постоянно сталкивался с проблемой, что при опредленном значении EPS
 * задача ЛП решалась правильно, а при другом значении она не решалась.
 * В данном решении, как мне кажется, поле solusion_vars является костылем,
 * так как симплексная таблица в конце преобразований не всегда имеет ту форму,
 * когда в нужных с точки зрения грейдера столбцах присутствует только одна единица.
 * Поле solusion_vars как бы "исключает" некоторые значения из столбцов при подсчете
 * количества единиц в столбце.
 * 
 * С моей стороны произведен некоторый рефакторинг кода, как доказательство того,
 * что алгоритм я разобрал, понял и пытался самостоятельно реализовать. В процессе
 * рефакторинга удалено дублирование кода, ненужные операции и т. д.
 * 
*/

#include <algorithm>
#include <cstdio>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

const long double EPS = std::numeric_limits<long double>::epsilon();

enum class state
{
  optimal,
  infeasible,
  unbounded
};

enum class method_phase
{
  one,
  two
};

using matrix = std::vector<std::vector<long double>>;

using column = std::vector<long double>;
using row = std::vector<long double>;

struct position
{
  int row;
  int column;
  bool is_optimal() { return row == -1 || column == -1; }
};

struct simplex_method
{

  void debug_print() const
  {
    for (std::size_t i = 0; i < A.size(); ++i)
    {
      for (std::size_t j = 0; j < A[i].size(); ++j)
      {
        std::cout << std::fixed << std::setw(5) << std::setprecision(2) << A[i][j] << ' ';
      }
      std::cout << std::fixed << std::setw(5) << " | " << std::setprecision(2) << b[i] << std::endl;
    }

    for (const auto& v : c)
    {
      std::cout << std::setw(7) << v << ' ';
    }

    std::cout << " | " << b[b.size() - 2] << std::endl;

    if (phase == method_phase::one)
    {
      for (const auto& v : w)
      {
        std::cout << std::setw(5) << v << ' ';
      }

      std::cout << " | " << b.back() << std::endl;
    }
  }

  void handle_artficial_vars()
  {
    for (std::size_t i = 0, j = m + n; i < b.size() - 1; ++i, ++j)
    {
      if (b[i] < 0.0)
      {

        solusion_vars[i] = -2;
        A[i][j] = -1;

        b.back() += b[i];
        b[i] = -b[i];

        std::transform(A[i].begin(), A[i].end(), A[i].begin(), std::negate<long double>());

        for (int k = 0; k < n + m; ++k)
        {
          w[k] += A[i][k];
        }
      }
    }

    std::transform(w.begin(), w.end(), w.begin(), std::negate<long double>());
  }

  void handle_slack_vars()
  {
    for (std::size_t i = 0; i < A.size(); ++i)
    {
      A[i][i + m] = 1;
    }
  }

  // Find pivots and run eliminations to get optimal objective
  void Gauss_Jordam_eliminations(row& obj)
  {
    while (true)
    {
      position p = find_pivot(obj);

      if (p.is_optimal() || cur_solution == state::unbounded)
      {
        break;
      }

      solusion_vars[p.column] = p.row;

      scale_pivot(p);

      process_pivot(p);
    }
  }

  void trim_table_from_avars()
  {
    c.resize(c.size() - n);
    b.pop_back();

    for (auto& r : A)
    {
      r.resize(r.size() - n);
    }
  }

  void phase_two()
  {
    phase = method_phase::two;
    trim_table_from_avars();
    Gauss_Jordam_eliminations(c);
  }

  void phase_one()
  {
    phase = method_phase::one;
    Gauss_Jordam_eliminations(w);
    cur_solution = double_equals_zero(b.back()) ? state::optimal : state::infeasible;
  }

  void prepare_table()
  {
    solusion_vars = std::vector<int>(A[0].size(), -1);
    std::transform(c.begin(), c.end(), c.begin(), std::negate<double>());
    handle_slack_vars();
    if (mc)
    {
      w = row(c.size());
      handle_artficial_vars();
    }
  }

  bool double_equals(double a, double b, double epsilon = 0.001)
  {
    return std::abs(a - b) < epsilon;
  }

  bool double_equals_zero(long double a, long double epsilon = 0.001)
  {
    return double_equals(a, 0.0, epsilon);
  }

  bool mixed_constraints() const
  {
    auto it = std::find_if(b.cbegin(), b.cend(), [](auto j) { return j < 0.0; });
    return it == b.cend() ? false : true;
  }

  std::pair<int, std::vector<long double>> read_optimal_solution()
  {
    std::vector<long double> result(m);

    for (int i = 0; i < m; ++i)
    {
      long double sum = 0.0;
      int k = 0;
      for (std::size_t j = 0; j < A.size(); ++j)
      {
        if (solusion_vars[j] >= 0.0)
          sum += fabs(A[j][i]);
        if (double_equals(A[j][i], 1.0))
        {
          k = j;
        }
      }

      result[i] = (sum - 1.0 > EPS) ? 0.0 : b[k];
    }

    return { 0, std::move(result) };
  }

  std::pair<int, std::vector<long double>> solve()
  {
    mc = mixed_constraints();

    prepare_table();

    if (mc)
    {
      phase_one();
      if (cur_solution == state::infeasible)
      {
        return { -1, {} };
      }
    }

    phase_two();

    if (cur_solution == state::unbounded)
    {
      return { 1, {} };
    }

    return read_optimal_solution();
  }

  position find_pivot(row& cw) {
    int i = 0;
    int j = distance(cw.begin(), min_element(cw.begin(), cw.end()));
    long double ratio = std::numeric_limits<long double>::max();
    
    if (cw[j] >= 0.0) {
      return { -1, -1 };
    }
    
    for (std::size_t k = 0; k < A.size(); ++k) {
      if (A[k][j] > 0.0 || A[k][j] < 0.0) {
        const long double r = b[k] / A[k][j];
        if (r > 0.0 && r - ratio < EPS) {
          ratio = r;
          i = static_cast<int>(k);
        }
      }
    }

    if (ratio == std::numeric_limits<long double>::max()) {
      cur_solution = state::unbounded;
    }
    
    return { i, j };
  }
  
  void process_pivot(position p) {
    for (std::size_t i = 0; i < A.size(); ++i) {
      if (p.row != i && !double_equals_zero(A[i][p.column], EPS)) {
        const long double m = A[i][p.column];
        for (std::size_t j = 0; j < A[0].size(); ++j) {
          A[i][j] -= m * A[p.row][j];
        }
        b[i] -= m * b[p.row];
      }
    }
    
    if (phase == method_phase::one) {
      const long double mw = w[p.column];
      for (std::size_t i = 0; i < w.size(); ++i) {
        w[i] -= A[p.row][i] * mw;
      }
      b[b.size() - 1] -= mw * b[p.row];
    }

    const long double cw = c[p.column];
    for (std::size_t i = 0; i < c.size(); ++i) {
      c[i] -= A[p.row][i] * cw;
    }
    b[n] -= cw * b[p.row];
  }

  void scale_pivot(position p)
  {
    auto d = A[p.row][p.column];
    b[p.row] /= d;
    for (auto& n : A[p.row])
    {
      n /= d;
    }
  }

  int n, m;
  matrix A;
  std::vector<long double> b, c, w;
  std::vector<int> solusion_vars;
  state cur_solution;
  method_phase phase;
  bool mc;
};

int main() {
  int n, m;
  std::cin >> n >> m;
  
  matrix A(n, std::vector<long double>(n + m + n, 0.0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cin >> A[i][j];
    }
  }
  
  std::vector<long double> b(n + 2);
  for (int i = 0; i < n; i++) {
    std::cin >> b[i];
  }
  
  std::vector<long double> c(n + m + n);
  for (int i = 0; i < m; i++) {
    std::cin >> c[i];
  }
  
  simplex_method sm{ n, m, std::move(A), std::move(b), std::move(c) };
  std::pair<int, std::vector<long double>> ans = sm.solve();
  
  switch (ans.first) {
  case -1:
    printf("No solution\n");
    break;
  case 0:
    printf("Bounded solution\n");
    for (int i = 0; i < m; i++) {
      printf("%.18Lf%c", ans.second[i], " \n"[i + 1 == m]);
    }
    break;
  case 1:
    printf("Infinity\n");
    break;
  }

  return 0;
}
