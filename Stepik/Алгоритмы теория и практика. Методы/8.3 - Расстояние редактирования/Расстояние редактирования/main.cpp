#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int Diff(char c1, char c2) {
  return c1 == c2 ? 0 : 1;
}

int Min(int n1, int n2, int n3) {
  return n1 <= n2 ? std::min(n1, n3) : std::min(n2, n3);
}

int main()
{
  std::string str_a;
  std::string str_b;
  std::cin >> str_a >> str_b;

  int str_a_len = str_a.length();
  int str_b_len = str_b.length();

  std::vector<std::vector<int>> dynamic(str_a_len + 1);
  for (auto& row : dynamic) {
    row.resize(str_b_len + 1);
  }
  
  for (int j = 0; j <= str_b_len; ++j) {
    dynamic[0][j] = j;
  }

  for (int i = 0; i <= str_a_len; ++i) {
    dynamic[i][0] = i;
  }

  for (int i = 1; i <= str_a_len; ++i)
  {
    for (int j = 1; j <= str_b_len; ++j)
    {
      int diff = Diff(str_a[i - 1], str_b[j - 1]);
      dynamic[i][j] = Min(dynamic[i][j - 1] + 1, dynamic[i - 1][j] + 1, dynamic[i - 1][j - 1] + diff);
    }
  }

  std::cout << dynamic[str_a_len][str_b_len];
  return 0;
}
