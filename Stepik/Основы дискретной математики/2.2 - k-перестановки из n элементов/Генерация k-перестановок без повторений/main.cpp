#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <vector>

using namespace std;

class Solution {
public:
  Solution(int n, int k) : m_n(n), m_k(k) {
    if (k < 0 || k > n) {
      throw invalid_argument("Invalid argument k");
    }

    if (n < 0) {
      throw invalid_argument("Invalid argument n");
    }
  }

  vector<vector<int>> GetPermutations() const {
    vector<int> permutation;
    vector<vector<int>> permutations;
    backtrack(permutation, permutations);
    return permutations;
  }

private:
  void backtrack(
    vector<int>& permutation,
    vector<vector<int>>& permutations) const
  {
    if ((int)permutation.size() == m_k) {
      permutations.push_back(permutation);
      return;
    }
    
    for (int i = 0; i < m_n; ++i) {
      if (!m_used_numbers.count(i)) {
        permutation.push_back(i);
        m_used_numbers.insert(i);

        backtrack(permutation, permutations);

        permutation.pop_back();
        m_used_numbers.erase(i);
      }
    }
  }

private:
  const int m_n;
  const int m_k;
  mutable unordered_set<int> m_used_numbers;
};

int main() {
  int n, k;
  cin >> n >> k;

  Solution solution(n, k);
  auto choices = solution.GetPermutations();

  for (const auto& row : choices) {
    for (int num : row) {
      cout << num << ' ';
    }
    cout << '\n';
  }

  return 0;
}
