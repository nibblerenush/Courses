#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Graph {
public:
  Graph(int v, int e) : m_vertex_count(v), m_edge_count(e) {
    auto check_arg = [](int arg, int max_val, const string& msg) {
      if (arg < 0 || arg > max_val) {
        throw invalid_argument(msg);
      }
    };

    check_arg(v, 10000, "Invalid value of v");
    check_arg(e, 30000, "Invalid value of e");

    ConstructGraph();
  }

  vector<int> GetDistances() const {
    if (m_vertex_count == 0) {
      return vector<int>{};
    }

    return bfs();
  }

private:
  vector<int> bfs() const {
    vector<int> distances(m_vertex_count, -1);
    distances[0] = 0;

    queue<int> work_queue;
    work_queue.push(0);

    while (!work_queue.empty()) {
      int v = work_queue.front();
      work_queue.pop();

      for (int u : m_adjacency[v]) {
        if (distances[u] == -1) {
          distances[u] = distances[v] + 1;
          work_queue.push(u);
        }
      }
    }

    return distances;
  }

  void ConstructGraph() {
    m_adjacency.resize(m_vertex_count);

    for (int i = 0; i < m_edge_count; ++i) {
      int u, v;
      cin >> u >> v;
      m_adjacency[u].push_back(v);
      m_adjacency[v].push_back(u);
    }
  }

private:
  const int m_vertex_count;
  const int m_edge_count;
  vector<vector<int>> m_adjacency;
};

int main() {
  int v, e;
  cin >> v >> e;

  Graph graph(v, e);
  auto distances = graph.GetDistances();
  for (int distance : distances) {
    cout << distance << ' ';
  }

  return 0;
}

/*
6 7
0 1
1 2
2 0
3 2
4 3
4 2
5 4
*/
