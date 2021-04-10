#include <iostream>
#include <limits>
#include <vector>
#include <queue>

static const long long infinite = std::numeric_limits<long long>::max();

using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;

void shortest_paths(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, vector<long long> &distance, vector<int> &reachable, vector<int> &shortest) {
  //write your code here
  distance[s] = 0;
  
  // v - 1 times
  for (size_t i = 0; i < adj.size() - 1; ++i) {
    // for all edges
    for (size_t v = 0; v < adj.size(); ++v) {
      for (int w : adj[v]) {
        // relax
        if (distance[v] != infinite && distance[w] > distance[v] + cost[v][w]) {
          distance[w] = distance[v] + cost[v][w];
        }
      }
    }
  }
  
  queue<int> work_queue;
  vector<long long> work_distance(adj.size(), infinite);
  
  // v (last) time
  for (size_t v = 0; v < adj.size(); ++v) {
    for (int w : adj[v]) {
      if (distance[v] != infinite && distance[w] > distance[v] + cost[v][w]) {
        work_queue.push(w);
        work_distance[w] = 0;
      }
    }
  }

  // get reachable
  for (size_t i = 0; i < distance.size(); ++i) {
    if (distance[i] != infinite) {
      reachable[i] = 1;
    }
  }
  
  while (!work_queue.empty()) {
    int v = work_queue.front();
    work_queue.pop();
    
    for (int w : adj[v]) {
      if (work_distance[w] == infinite) {
        work_queue.push(w);
        work_distance[w] = 0;
      }
    }
  }

  for (size_t i = 0; i < work_distance.size(); ++i) {
    if (work_distance[i] == 0) {
      shortest[i] = 0;
    }
  }
}

int main() {
  int n, m, s;
  
  std::cin >> n >> m;
  vector<vector<int>> adj(n, vector<int>());
  vector<vector<int>> cost(n, vector<int>());
  for (auto& c : cost) {
    c.assign(n, std::numeric_limits<int>::max());
  }

  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    if (w < cost[x - 1][y - 1]) {
      cost[x - 1][y - 1] = w;
    }
  }

  std::cin >> s;
  s--;
  vector<long long> distance(n, infinite);
  vector<int> reachable(n, 0);
  vector<int> shortest(n, 1);
  shortest_paths(adj, cost, s, distance, reachable, shortest);
  for (int i = 0; i < n; i++) {
    if (!reachable[i]) {
      std::cout << "*\n";
    } else if (!shortest[i]) {
      std::cout << "-\n";
    } else {
      std::cout << distance[i] << "\n";
    }
  }
}
