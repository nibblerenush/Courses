#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

using Pair = std::pair<char, std::size_t>;

std::size_t letterToIndex(char letter) {
  switch (letter) {
  case '$': return 0; break;
  case 'A': return 1; break;
  case 'C': return 2; break;
  case 'G': return 3; break;
  case 'T': return 4; break;
  }
}

char indexToLetter(std::size_t index) {
  switch (index) {
  case 0: return '$'; break;
  case 1: return 'A'; break;
  case 2: return 'C'; break;
  case 3: return 'G'; break;
  case 4: return 'T'; break;
  }
}

std::vector<Pair> GetColumn(const std::string& column_str) {
  std::unordered_map<char, std::size_t> help_hash_table;
  
  std::vector<Pair> column;
  column.reserve(column_str.size());
  
  for (char c : column_str) {
    help_hash_table[c]++;
    column.push_back({ c, help_hash_table[c] });
  }
  return column;
}

string InverseBWT(const string& bwt) {
  string text = "";
  // write your code here
  
  text.reserve(bwt.size());
  auto last_column = GetColumn(bwt);
  
  // sort bwt
  std::vector<std::size_t> char_count(5, 0);
  for (char c : bwt) {
    char_count[letterToIndex(c)]++;
  }
  std::string first_column_str;
  first_column_str.reserve(bwt.size());
  for (std::size_t i = 0; i < char_count.size(); ++i) {
    for (std::size_t j = 0; j < char_count[i]; ++j) {
      first_column_str.push_back(indexToLetter(i));
    }
  }

  auto first_column = GetColumn(first_column_str);

  std::map<Pair, Pair> first_last;
  for (std::size_t i = 0; i < bwt.size(); ++i) {
    first_last[first_column[i]] = last_column[i];
  }
  
  auto iter = first_last.begin();
  text.push_back(iter->first.first);
  for (std::size_t i = 0; i < bwt.size() - 1; ++i) {
    text.push_back(iter->second.first);
    iter = first_last.find(iter->second);
  }
  std::reverse(text.begin(), text.end());
  return text;
}

int main() {
  string bwt;
  cin >> bwt;
  cout << InverseBWT(bwt) << endl;
  return 0;
}
