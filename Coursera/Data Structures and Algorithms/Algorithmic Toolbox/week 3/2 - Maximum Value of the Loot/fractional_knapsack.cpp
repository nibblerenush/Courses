#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

double get_optimal_value(int capacity, vector<int> weights, vector<int> values) {
  using namespace std;

  // prepare
  double result_value = 0.0;
  const int size = weights.size();

  // preprocessing
  vector<pair<double, int>> value_per_weight(size);
  for (int i = 0; i < size; ++i) {
    value_per_weight[i] = { (double)values[i] / weights[i], i };
  }
  sort(value_per_weight.begin(), value_per_weight.end(), greater<pair<double, int>>());

  // algorithm
  for (int i = 0; i < size; ++i) {
    if (capacity == 0) {
      break;
    }
    
    int value = values[value_per_weight[i].second];
    int weight = weights[value_per_weight[i].second];
    
    int a = min(weight, capacity);
    result_value += ((double)a * value / weight);
    capacity -= a;
  }

  return result_value;
}

int main() {
  int n;
  int capacity;
  std::cin >> n >> capacity;
  vector<int> values(n);
  vector<int> weights(n);
  for (int i = 0; i < n; i++) {
    std::cin >> values[i] >> weights[i];
  }

  double optimal_value = get_optimal_value(capacity, weights, values);

  std::cout.precision(10);
  std::cout << optimal_value << std::endl;
  return 0;
}
