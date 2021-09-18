#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  int N = 0;
  std::cin >> N;

  std::vector<std::pair<int, int>> time_index;
  time_index.reserve(N);

  for (int i = 0; i < N; ++i) {
    int T = 0;
    std::cin >> T;
    time_index.emplace_back(T, i);
  }

  std::sort(time_index.begin(), time_index.end());

  for (int i = 0; i < N; ++i) {
    std::cout << time_index[i].second << ' ';
  }

  return 0;
}
