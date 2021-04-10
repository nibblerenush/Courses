#include <string>
#include <iostream>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::string;

typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(vector<string> & patterns) {
  trie t;
  // write your code here
  
  // root node - number is 0
  t.push_back(edges{});
  
  for (const auto& pattern : patterns) {
    int current_node = 0;
    for (const auto& c : pattern) {
      if (t[current_node].count(c)) {
        // next node
        current_node = t[current_node][c];
      }
      else {
        // new node withous edges
        t.push_back(edges{});

        // new edge from cuurent_node to new node
        t[current_node].emplace(c, t.size() - 1);

        // next node
        current_node = t.size() - 1;
      }
    }
  }

  return t;
}

int main() {
  size_t n;
  std::cin >> n;
  vector<string> patterns;
  for (size_t i = 0; i < n; i++) {
    string s;
    std::cin >> s;
    patterns.push_back(s);
  }

  trie t = build_trie(patterns);
  for (size_t i = 0; i < t.size(); ++i) {
    for (const auto & j : t[i]) {
      std::cout << i << "->" << j.second << ":" << j.first << "\n";
    }
  }

  return 0;
}
