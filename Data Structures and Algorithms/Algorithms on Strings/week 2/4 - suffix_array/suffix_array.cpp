#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<int> BuildSuffixArray(const string& text) {
  vector<int> result;
  // Implement this function yourself
  result.resize(text.size());
  
  std::vector<string> matrix;
  matrix.reserve(text.size());
  matrix.push_back(text);
  
  for (std::size_t i = 1; i < text.size(); ++i) {
    std::string temp = matrix[i - 1];
    std::rotate(temp.begin(), std::prev(temp.end()), temp.end());
    matrix.push_back(std::move(temp));
  }
  std::sort(matrix.begin(), matrix.end());

  for (std::size_t i = 0; i < text.size(); ++i) {
    const std::string& row = matrix[i];
    std::size_t dollar_sign_pos = row.find('$');
    result[i] = text.size() - 1 - dollar_sign_pos;
  }

  return result;
}

int main() {
  string text;
  cin >> text;
  vector<int> suffix_array = BuildSuffixArray(text);
  for (int i = 0; i < suffix_array.size(); ++i) {
    cout << suffix_array[i] << ' ';
  }
  cout << endl;
  return 0;
}
