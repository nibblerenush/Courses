#include <iostream>
#include <numeric>
#include <vector>

using std::vector;

vector<vector<int>> knapsack(const vector<int>& A, int sum) {
  vector<vector<int>> dynamic(sum + 1, vector<int>(A.size() + 1, 0));
  
  for (int s = 1; s <= sum; ++s) {
    for (int i = 1; i <= (int)A.size(); ++i) {
      dynamic[s][i] = dynamic[s][i - 1];
      if (A[i - 1] <= s) {
        dynamic[s][i] = std::max(dynamic[s][i], dynamic[s - A[i - 1]][i - 1] + A[i - 1]);
      }
    }
  }
  
  return dynamic;
}

int partition3(vector<int> &A) {
  int sum = std::accumulate(A.begin(), A.end(), 0);
  if (sum % 3 != 0) {
    return 0;
  }
  sum = sum / 3;
  
  auto dynamic = knapsack(A, sum);
  if (dynamic[sum][A.size()] != sum) {
    return 0;
  }

  int s = sum;
  int i = A.size();
  while (i >= 1) {
    if (dynamic[sum][i] != dynamic[sum][i - 1]) {
      sum -= A[i - 1];
      A[i - 1] = 0;
    }
    i -= 1;
  }
  
  dynamic = knapsack(A, sum);
  if (dynamic[sum][A.size()] != sum) {
    return 0;
  }
  return 1;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> A(n);
  for (size_t i = 0; i < A.size(); ++i) {
    std::cin >> A[i];
  }
  std::cout << partition3(A) << '\n';
}
