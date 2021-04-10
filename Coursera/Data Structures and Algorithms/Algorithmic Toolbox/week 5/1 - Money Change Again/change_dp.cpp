#include <iostream>
#include <limits>
#include <vector>

int get_change(int m) {
  using namespace std;
  const int infinity = numeric_limits<int>::max();
  vector<int> dynamic(m + 1, infinity);
  dynamic[0] = 0;

  for (int i = 1; i <= m; ++i) {  
    dynamic[i] = min(dynamic[i], i >= 1 ? dynamic[i - 1] : infinity);
    dynamic[i] = min(dynamic[i], i >= 3 ? dynamic[i - 3] : infinity);
    dynamic[i] = min(dynamic[i], i >= 4 ? dynamic[i - 4] : infinity);
    dynamic[i] += 1;
  }

  return dynamic[m];
}

int main() {
  int m;
  std::cin >> m;
  std::cout << get_change(m) << '\n';
}
