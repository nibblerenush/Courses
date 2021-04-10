#include <iostream>
#include <limits>
#include <vector>

static const int infinite = std::numeric_limits<int>::max();

using std::vector;

int negative_cycle(vector<vector<int>>& adj, vector<vector<int>>& cost) {
  //write your code here
  vector<int> distances(adj.size(), infinite);
  
  for (size_t i = 0; i < distances.size(); ++i) {
    if (distances[i] == infinite) {
      // cur vertex is i
      distances[i] = 0;
      
      // v - 1 times
      for (size_t j = 0; j < adj.size() - 1; ++j) {
        // for all edges
        for (size_t v = 0; v < adj.size(); ++v) {
          for (int w : adj[v]) {
            // relax
            if (distances[v] != infinite && distances[w] > distances[v] + cost[v][w]) {
              distances[w] = distances[v] + cost[v][w];
            }
          }
        }
      }
      
      // v (last) time
      for (size_t v = 0; v < adj.size(); ++v) {
        for (int w : adj[v]) {
          if (distances[v] != infinite && distances[w] > distances[v] + cost[v][w]) {
            return 1;
          }
        }
      }
    }
  }

  return 0;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<vector<int>> adj(n, vector<int>());
  vector<vector<int>> cost(n, vector<int>());
  for (auto& c : cost) {
    c.assign(n, infinite);
  }

  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    if (w < cost[x - 1][y - 1]) {
      cost[x - 1][y - 1] = w;
    }
  }

  std::cout << negative_cycle(adj, cost);
}
