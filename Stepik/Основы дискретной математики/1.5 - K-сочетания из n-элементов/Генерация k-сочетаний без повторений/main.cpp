#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

class BinomialCoefficient {
public:
  BinomialCoefficient(int k, int n) : m_k(k), m_n(n) {
    if (k < 0 || k > n) {
      throw invalid_argument("Invalid argument k");
    }

    if (n < 0) {
      throw invalid_argument("Invalid argument n");
    }
  }

  vector<vector<int>> GetChoices() const {
    vector<int> choice;
    vector<vector<int>> choices;
    backtrack(0, 0, choice, choices);
    return choices;
  }

private:
  void backtrack(
    int begin_index,
    int depth,
    vector<int>& choice,
    vector<vector<int>>& choices) const
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

int main() {
  int k, n;
  cin >> k >> n;

  BinomialCoefficient bc(k, n);
  auto choices = bc.GetChoices();

  for (const auto& row : choices) {
    for (int num : row) {
      cout << num << ' ';
    }
    cout << '\n';
  }

  return 0;
}
