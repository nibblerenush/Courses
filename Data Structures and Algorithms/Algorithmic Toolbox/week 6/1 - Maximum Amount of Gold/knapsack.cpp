#include <iostream>
#include <vector>

using std::vector;

int optimal_weight(int W, const vector<int> &w) {
  vector<vector<int>> dynamic(W + 1, vector<int>(w.size() + 1, 0));

  for (int weight = 1; weight <= W; ++weight) {
    for (int i = 1; i <= (int)w.size(); ++i) {
      dynamic[weight][i] = dynamic[weight][i - 1];
      if (w[i - 1] <= weight) {
        dynamic[weight][i] = std::max(dynamic[weight][i], dynamic[weight - w[i - 1]][i - 1] + w[i - 1]);
      }
    }
  }

  return dynamic[W][w.size()];
}

int main() {
  int n, W;
  std::cin >> W >> n;
  vector<int> w(n);
  for (int i = 0; i < n; i++) {
    std::cin >> w[i];
  }
  std::cout << optimal_weight(W, w) << '\n';
}
