#include <iostream>
#include <vector>

using std::vector;
using std::pair;

void explore(vector<vector<int>>& adj, vector<bool>& visited, bool& has_cycle, int v) {
  if (has_cycle) {
    return;
  }
  
  visited[v] = true;
  for (int w : adj[v]) {
    if (!visited[w]) {
      explore(adj, visited, has_cycle, w);
    }
    else {
      has_cycle = true;
    }
  }
  visited[v] = false;
}

int acyclic(vector<vector<int>>& adj) {
  //write your code here
  vector<bool> visited(adj.size(), false);
  for (size_t v = 0; v < adj.size(); ++v) {  
    bool has_cycle {false};
    explore(adj, visited, has_cycle, v);
    if (has_cycle) {
      return 1;
    }
  }
  return 0;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
  }
  std::cout << acyclic(adj);
  return 0;
}
