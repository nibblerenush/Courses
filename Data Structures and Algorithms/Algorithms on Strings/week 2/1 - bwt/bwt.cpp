#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

string BWT(const string& text) {
  string result = "";
  result.reserve(text.size());

  // write your code here
  std::vector<string> matrix;
  matrix.reserve(text.size());
  matrix.push_back(text);
  
  for (std::size_t i = 1; i < text.size(); ++i) {
    std::string temp = matrix[i - 1];
    std::rotate(temp.begin(), std::prev(temp.end()), temp.end());
    matrix.push_back(std::move(temp));
  }

  std::sort(matrix.begin(), matrix.end());
  for (const std::string& row : matrix) {
    result.push_back(row.back());
  }

  return result;
}

int main() {
  string text;
  cin >> text;
  cout << BWT(text) << endl;
  return 0;
}
