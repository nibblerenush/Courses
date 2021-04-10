#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;

std::vector<int> PrefixFunction(const std::string& str) {
  std::vector<int> result(str.size());

  result[0] = 0;
  int border = 0;

  for (int i = 1; i < static_cast<int>(str.size()); ++i) {
    while (border > 0 && str[i] != str[border]) {
      border = result[border - 1];
    }

    if (str[i] == str[border]) {
      border += 1;
    }
    else {
      border = 0;
    }

    result[i] = border;
  }

  return result;
}

// Find all occurrences of the pattern in the text and return a
// vector with all positions in the text (starting from 0) where 
// the pattern starts in the text.
vector<int> find_pattern(const string& pattern, const string& text) {
  vector<int> result;
  // Implement this function yourself

  std::string temp = pattern + '$' + text;
  auto prefix_func_result = PrefixFunction(temp);

  for (std::size_t i = pattern.size() + 1; i < temp.size(); ++i) {
    if (prefix_func_result[i] == pattern.size()) {
      result.push_back(static_cast<int>(i - 2 * pattern.size()));
    }
  }

  return result;
}

int main() {
  string pattern, text;
  cin >> pattern;
  cin >> text;
  vector<int> result = find_pattern(pattern, text);
  for (int i = 0; i < result.size(); ++i) {
    printf("%d ", result[i]);
  }
  printf("\n");
  return 0;
}
