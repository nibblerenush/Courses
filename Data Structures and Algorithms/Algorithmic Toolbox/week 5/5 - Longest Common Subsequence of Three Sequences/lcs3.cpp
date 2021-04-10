#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

int lcs3(vector<int> &a, vector<int> &b, vector<int> &c) {
  const int an = a.size();
  const int bn = b.size();
  const int cn = c.size();

  vector<vector<vector<int>>> dynamic(an + 1, vector<vector<int>>(bn + 1, vector<int>(cn + 1, 0)));

  for (int i = 1; i <= an; ++i) {
    for (int j = 1; j <= bn; ++j) {
      for (int k = 1; k <= cn; ++k) {
        dynamic[i][j][k] = std::max({ dynamic[i - 1][j][k], dynamic[i][j - 1][k], dynamic[i][j][k - 1] });
        if (a[i - 1] == b[j - 1] && a[i - 1] == c[k - 1]) {
          dynamic[i][j][k] = std::max(dynamic[i][j][k], dynamic[i - 1][j - 1][k - 1] + 1);
        }
      }
    }
  }

  return dynamic[an][bn][cn];
}

int main() {
  size_t an;
  std::cin >> an;
  vector<int> a(an);
  for (size_t i = 0; i < an; i++) {
    std::cin >> a[i];
  }
  size_t bn;
  std::cin >> bn;
  vector<int> b(bn);
  for (size_t i = 0; i < bn; i++) {
    std::cin >> b[i];
  }
  size_t cn;
  std::cin >> cn;
  vector<int> c(cn);
  for (size_t i = 0; i < cn; i++) {
    std::cin >> c[i];
  }
  std::cout << lcs3(a, b, c) << std::endl;
}
