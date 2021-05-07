#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

struct Point {
  int x = 0;
  int y = 0;
};

double get_euclidean_distance(int x1, int y1, int x2, int y2) {
  double a = ((double)x1 - x2) * ((double)x1 - x2);
  double b = ((double)y1 - y2) * ((double)y1 - y2);
  return sqrt(a + b);
}

long long get_distance(const vector<Point>& points, const vector<vector<int>>& adj, const vector<vector<int>>& costs, int s, int t) {
  /*vector<double> estimate(adj.size(), numeric_limits<double>::max());
  estimate[s] = 0;*/

  vector<long long> distance(adj.size(), numeric_limits<long long>::max());
  distance[s] = 0;

  // pair<double, int> - estimate, vertex
  using est_v_t = pair<double, int>;
  priority_queue<est_v_t, vector<est_v_t>, greater<est_v_t>> work_queue;
  work_queue.push({ 0, s });
  
  while (!work_queue.empty()) {
    int current = work_queue.top().second;
    work_queue.pop();

    if (current == t) {
      return distance[t];
    }

    for (int i = 0; i < (int)adj[current].size(); ++i) {
      const int w = adj[current][i];
      const int cost = costs[current][i];
      double potential = get_euclidean_distance(points[w].x, points[w].y, points[t].x, points[t].y);
      
      if (distance[w] > distance[current] + cost) {
        distance[w] = distance[current] + cost;
        work_queue.push({ distance[w] + potential, w });
      }
    }
  }

  return -1;
}

int main() {
  int n, m;
  cin >> n >> m;

  vector<Point> points(n);
  for (int i = 0; i < n; ++i) {
    int x, y;
    cin >> x >> y;
    points[i] = Point{ x, y };
  }

  vector<vector<int>> adj(n);
  vector<vector<int>> costs(n);
  for (int i = 0; i < m; ++i) {
    int u, v, l;
    cin >> u >> v >> l;
    adj[u - 1].push_back(v - 1);
    costs[u - 1].push_back(l);
  }

  int q;
  cin >> q;
  vector<long long> result(q);

  for (int i = 0; i < q; ++i) {
    int s, t;
    cin >> s >> t;
    result[i] = get_distance(points, adj, costs, s - 1, t - 1);
  }

  for (int num : result) {
    cout << num << '\n';
  }

  return 0;
}
