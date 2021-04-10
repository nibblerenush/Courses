#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

struct CompareStruct {
  int operations_count = 0;
  int middle_result = 0;
};

bool operator<(const CompareStruct& lhs, const CompareStruct& rhs) {
  return lhs.operations_count < rhs.operations_count;
}

bool operator<=(const CompareStruct& lhs, const CompareStruct& rhs) {
  return lhs.operations_count <= rhs.operations_count;
}

vector<int> optimal_sequence(int n) {
  vector<int> dynamic(n + 1);
  dynamic[0] = dynamic[1] = 0;

  for (int i = 2; i <= n; ++i) {
    int min1 = i % 2 == 0 ? dynamic[i / 2] + 1 : std::numeric_limits<int>::max();
    int min2 = i % 3 == 0 ? dynamic[i / 3] + 1 : std::numeric_limits<int>::max();
    int min3 = dynamic[i - 1] + 1;
    dynamic[i] = std::min({ min1, min2, min3 });
  }
  
  vector<int> sequence;
  sequence.reserve(dynamic[n] + 1);
  
  int operations_count = dynamic[n];
  while (operations_count != 0) {
    sequence.push_back(n);

    CompareStruct min1 = n % 2 == 0 ?
      CompareStruct{dynamic[n / 2], n / 2} : CompareStruct{std::numeric_limits<int>::max(), 0};
      
    CompareStruct min2 = n % 3 == 0 ?
      CompareStruct{dynamic[n / 3], n / 3} : CompareStruct{std::numeric_limits<int>::max(), 0};
      
    CompareStruct min3 = {dynamic[n - 1], n - 1};
    
    CompareStruct min = std::min({ min1, min2, min3 });
    
    n = min.middle_result;
    operations_count = min.operations_count;
  }
  sequence.push_back(n);
  std::reverse(sequence.begin(), sequence.end());

  return sequence;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> sequence = optimal_sequence(n);
  std::cout << sequence.size() - 1 << std::endl;
  for (size_t i = 0; i < sequence.size(); ++i) {
    std::cout << sequence[i] << " ";
  }
}
