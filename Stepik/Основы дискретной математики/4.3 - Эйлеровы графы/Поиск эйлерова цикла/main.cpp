#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Graph {
private:
  using adjacency_t = unordered_map<int, unordered_map<int, int>>;

public:
  Graph(int v, int e) : m_vertex_count(v), m_edge_count(e) {
    auto check_arg = [](int arg, int min_val, int max_val, const string& msg) {
      if (arg < min_val || arg > max_val) {
        throw invalid_argument(msg);
      }
    };
    
    check_arg(v, 2, 1000, "Invalid value of v");
    check_arg(e, 0, 1000, "Invalid value of e");
    
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
  
  bool IsAllVerticesHasEvenDegree() const {
    for (const auto& u : m_adjacency) {
      int degree = 0;
      for (const auto& v : u.second) {
        degree += v.second;
      }

      if (degree % 2 != 0) {
        return false;
      }
    }

    return true;
  }

  vector<int> GetEulerianCycle() const {
    vector<int> eulerian_cycle;
    adjacency_t adjacency = m_adjacency;
    GetEulerianCycleImpl(0, adjacency, eulerian_cycle);
    return eulerian_cycle;
  }

private:
  void ConstructGraph() {
    for (int i = 0; i < m_edge_count; ++i) {
      int u, v;
      cin >> u >> v;
      m_adjacency[u - 1][v - 1] += 1;
      m_adjacency[v - 1][u - 1] += 1;
    }
  }

  void dfs(int v, vector<bool>& visited) const {
    if (!visited[v]) {
      visited[v] = true;

      if (m_adjacency.count(v)) {
        for (const auto& u : m_adjacency.at(v)) {
          dfs(u.first, visited);
        }
      }
    }
  }

  void GetEulerianCycleImpl(int v, adjacency_t& adjacency, vector<int>& eulerian_cycle) const {
    while (!adjacency[v].empty()) {
      int u = adjacency[v].begin()->first;

      --adjacency[v].begin()->second;
      --adjacency[u][v];

      if (adjacency[v].begin()->second == 0) {
        adjacency[v].erase(adjacency[v].begin());
        adjacency[u].erase(v);
      }

      GetEulerianCycleImpl(u, adjacency, eulerian_cycle);
    }
    
    eulerian_cycle.push_back(v);
  }

private:
  const int m_vertex_count;
  const int m_edge_count;
  adjacency_t m_adjacency;
};

int main() {
  int v, e;
  cin >> v >> e;
  Graph graph(v, e);
  
  if (graph.GetConnectedComponents() != 1 || !graph.IsAllVerticesHasEvenDegree()) {
    cout << "NONE";
    return 0;
  }

  auto eulerian_cycle = graph.GetEulerianCycle();
  eulerian_cycle.pop_back();
  for (int v : eulerian_cycle) {
    cout << v + 1 << ' ';
  }

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
1 4
*/

/*
3 3
1 2
2 3
3 1
*/

/*
8 14
1 2
1 4
4 3
3 2
3 2
3 5
2 5
5 5
4 6
4 8
6 8
6 8
7 6
8 6
*/

/*
5 8
1 3
3 1
1 5
4 3
5 1
5 2
2 4
3 5
*/
