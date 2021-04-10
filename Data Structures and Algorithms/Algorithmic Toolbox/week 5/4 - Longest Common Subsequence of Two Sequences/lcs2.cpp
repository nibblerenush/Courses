#include <iostream>
#include <vector>

using std::vector;

int lcs2(vector<int> &a, vector<int> &b) {
  const int n = a.size();
  const int m = b.size();

  vector<vector<int>> dynamic(n + 1, vector<int>(m + 1, 0));

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      dynamic[i][j] = std::max(dynamic[i][j - 1], dynamic[i - 1][j]);
      if (a[i - 1] == b[j - 1]) {
        dynamic[i][j] = std::max(dynamic[i][j], dynamic[i - 1][j - 1] + 1);
      }
    }
  }

  return dynamic[n][m];
}

int main() {
  size_t n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> a[i];
  }

  size_t m;
  std::cin >> m;
  vector<int> b(m);
  for (size_t i = 0; i < m; i++) {
    std::cin >> b[i];
  }

  std::cout << lcs2(a, b) << std::endl;
}
