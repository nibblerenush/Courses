#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using std::vector;
using std::queue;

int distance(vector<vector<int> > &adj, int s, int t) {
  //write your code here
  const int infinite = std::numeric_limits<int>::max();
  
  vector<int> distances(adj.size(), infinite);
  distances[s] = 0;
  
  queue<int> work_queue;
  work_queue.push(s);
  
  while (!work_queue.empty()) {
    int v = work_queue.front();
    work_queue.pop();
    
    for (int w : adj[v]) {
      if (distances[w] == infinite) {
        work_queue.push(w);
        distances[w] = distances[v] + 1;
      }
    }
  }

  return distances[t] == infinite ? -1 : distances[t];
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
  int s, t;
  std::cin >> s >> t;
  s--, t--;
  std::cout << distance(adj, s, t);
}
