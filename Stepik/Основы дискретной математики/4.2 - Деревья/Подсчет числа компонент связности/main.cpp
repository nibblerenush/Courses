#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Graph {
public:
  Graph(int v, int e) : m_vertex_count(v), m_edge_count(e) {
    auto check_arg = [] (int arg, const string& msg) {
      if (arg < 0 || arg > 1000) {
        throw invalid_argument(msg);
      }
    };

    check_arg(v, "Invalid value of v");
    check_arg(e, "Invalid value of e");

    ConstructGraph();
  }

  int GetConnectedComponents() const {
    int connected_components_count = 0;
    vector<bool> visited(m_vertex_count, false);

    for (int v = 0; v < m_vertex_count; ++v) {
      if (!visited[v]) {
        dfs(v, visited);
        connected_components_count += 1;
      }
    }

    return connected_components_count;
  }

private:
  void ConstructGraph() {
    m_adjacency.resize(m_vertex_count);

    for (int i = 0; i < m_edge_count; ++i) {
      int u, v;
      cin >> u >> v;
      m_adjacency[u - 1].push_back(v - 1);
      m_adjacency[v - 1].push_back(u - 1);
    }
  }

  void dfs(int v, vector<bool>& visited) const {
    if (!visited[v]) {
      visited[v] = true;
      for (int u : m_adjacency[v]) {
        dfs(u, visited);
      }
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
  cout << graph.GetConnectedComponents() << endl;

  return 0;
}

/*
4 2
1 2
3 2
*/

/*
4 3
1 2
3 2
4 3
*/
