#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;

static const int infinite = std::numeric_limits<int>::max();

long long distance(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, int t) {
  //write your code her

  vector<int> distances(adj.size(), infinite);
  distances[s] = 0;

  // pair<int, int> - distance, vertex
  using dist_v_t = pair<int, int>;
  priority_queue<dist_v_t, vector<dist_v_t>, std::greater<dist_v_t>> queue;
  queue.push({0, s});

  while (!queue.empty()) {
    dist_v_t item = queue.top();
    queue.pop();

    for (int w : adj[item.second]) {
      if (distances[w] > item.first + cost[item.second][w]) {
        distances[w] = item.first + cost[item.second][w];
        queue.push({distances[w], w});
      }
    }
  }

  return distances[t] != infinite ? distances[t] : -1;
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
  
  int s, t;
  std::cin >> s >> t;
  s--, t--;
  std::cout << distance(adj, cost, s, t);
}
