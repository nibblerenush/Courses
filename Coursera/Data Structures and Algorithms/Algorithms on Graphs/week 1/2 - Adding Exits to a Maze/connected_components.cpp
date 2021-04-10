#include <iostream>
#include <vector>

using std::vector;
using std::pair;

void Explore(vector<vector<int> > &adj, std::vector<bool>& visited, int v) {
  visited[v] = true;
  for (int w : adj[v]) {
    if (!visited[w]) {
      Explore(adj, visited, w);
    }
  }
}

int number_of_components(vector<vector<int> > &adj) {
  int res = 0;
  //write your code here
  std::vector<bool> visited(adj.size(), false);
  for (std::size_t v = 0; v < adj.size(); ++v) {
    if (!visited[v]) {
      Explore(adj, visited, v);
      res++;
    }
  }
  return res;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    adj[y - 1].push_back(x - 1);
  }
  std::cout << number_of_components(adj);
  return 0;
}
