#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

template <typename T>
T Min(T n1, T n2, T n3) {
  return n1 <= n2 ? std::min(n1, n3) : std::min(n2, n3);
}

struct CompareStruct
{
  int operations_count;
  int middle_result;
};

bool operator < (const CompareStruct& lhs, const CompareStruct& rhs) {
  return lhs.operations_count < rhs.operations_count;
}

bool operator <= (const CompareStruct& lhs, const CompareStruct& rhs) {
  return lhs.operations_count <= rhs.operations_count;
}

int main()
{
  int n;
  std::cin >> n;
  
  std::vector<int> dynamic(n + 1);
  dynamic[0] = dynamic[1] = 0;

  for (int i = 2; i <= n; ++i)
  {
    int min1 = i % 2 == 0 ? dynamic[i / 2] + 1 : std::numeric_limits<int>::max();
    int min2 = i % 3 == 0 ? dynamic[i / 3] + 1 : std::numeric_limits<int>::max();
    int min3 = dynamic[i - 1] + 1;
    dynamic[i] = Min(min1, min2, min3);
  }
  std::cout << dynamic[n] << '\n';

  std::vector<int> print_result;
  print_result.reserve(dynamic[n] + 1);

  int operations_count = dynamic[n];
  while (operations_count != 0)
  {
    print_result.push_back(n);

    CompareStruct min1 = n % 2 == 0 ?
      CompareStruct{dynamic[n / 2], n / 2} : CompareStruct{std::numeric_limits<int>::max(), 0};
      
    CompareStruct min2 = n % 3 == 0 ?
      CompareStruct{dynamic[n / 3], n / 3} : CompareStruct{std::numeric_limits<int>::max(), 0};
      
    CompareStruct min3 = {dynamic[n - 1], n - 1};
    
    CompareStruct min = Min(min1, min2, min3);
    
    n = min.middle_result;
    operations_count = min.operations_count;
  }
  print_result.push_back(n);
  std::reverse(print_result.begin(), print_result.end());

  for (const auto& middle_result : print_result) {
    std::cout << middle_result << ' ';
  }
  std::cout << std::endl;

  return 0;
}
