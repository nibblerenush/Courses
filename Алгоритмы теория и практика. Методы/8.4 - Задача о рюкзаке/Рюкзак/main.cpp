#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
  int knapsack_weight;
  int gold_count;
  std::cin >> knapsack_weight >> gold_count;

  std::vector<int> gold_weights(gold_count + 1);
  for (int i = 1; i <= gold_count; ++i) {
    std::cin >> gold_weights[i];
  }

  std::vector<std::vector<int>> dynamic(knapsack_weight + 1);
  for (auto& row: dynamic) {
    row.resize(gold_count + 1);
  }

  for (int i = 0; i <= gold_count; ++i) {
    dynamic[0][i] = 0;
  }

  for (int w = 0; w <= knapsack_weight; ++w) {
    dynamic[w][0] = 0;
  }
  
  for (int i = 1; i <= gold_count; ++i)
  {
    for (int w = 1; w <= knapsack_weight; ++w)
    {
      dynamic[w][i] = dynamic[w][i - 1];
      int gold_weight = gold_weights[i];
      
      if (gold_weight <= w)
      {
        dynamic[w][i] = std::max(dynamic[w][i],
                                 dynamic[w - gold_weight][i - 1] + gold_weight);
      }
    }
  }
  
  std::cout << dynamic[knapsack_weight][gold_count] << std::endl;
  return 0;
}
