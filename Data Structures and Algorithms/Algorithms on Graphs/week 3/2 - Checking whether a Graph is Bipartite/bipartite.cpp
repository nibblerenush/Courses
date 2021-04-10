#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using std::vector;
using std::queue;

int bipartite(vector<vector<int> > &adj) {
  //write your code here
  const int infinite = std::numeric_limits<int>::max();
  
  vector<int> distances(adj.size(), infinite);
  for (size_t i = 0; i < distances.size(); ++i) {
    if (distances[i] == infinite) {
      distances[i] = 0;
      
      queue<int> work_queue;
      work_queue.push(i);
      
      while (!work_queue.empty()) {
        int v = work_queue.front();
        work_queue.pop();
        
        for (int w : adj[v]) {
          if (distances[w] == infinite) {
            work_queue.push(w);
            distances[w] = distances[v] + 1;
          }
          else if (std::abs(distances[w] - distances[v]) % 2 == 0) {
            return 0;
          }
        }
      }
    }
  }
  return 1;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    adj[y - 1].push_back(x - 1);
  }
  std::cout << bipartite(adj);
}
