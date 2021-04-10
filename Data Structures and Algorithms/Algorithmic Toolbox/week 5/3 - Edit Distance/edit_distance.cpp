#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::string;

int diff_func(char c1, char c2) {
  return c1 == c2 ? 0 : 1;
}

int edit_distance(const string &str1, const string &str2) {
  int str_a_len = str1.length();
  int str_b_len = str2.length();

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

  for (int i = 1; i <= str_a_len; ++i) {
    for (int j = 1; j <= str_b_len; ++j) {
      int diff = diff_func(str1[i - 1], str2[j - 1]);
      dynamic[i][j] = std::min({ dynamic[i][j - 1] + 1, dynamic[i - 1][j] + 1, dynamic[i - 1][j - 1] + diff });
    }
  }

  return dynamic[str_a_len][str_b_len];
}

int main() {
  string str1;
  string str2;
  std::cin >> str1 >> str2;
  std::cout << edit_distance(str1, str2) << std::endl;
  return 0;
}
