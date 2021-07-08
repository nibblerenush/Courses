#include <iostream>
#include <limits>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Graph {
public:
  Graph(int v, int e) : m_vertex_count(v), m_edge_count(e) {
    auto check_arg = [](int arg, const string& msg) {
      if (arg < 0 || arg > 1000) {
        throw invalid_argument(msg);
      }
    };

    check_arg(v, "Invalid value of v");
    check_arg(e, "Invalid value of e");

    ConstructGraph();
  }

  bool IsBipartite() const {
    const int infinite = numeric_limits<int>::max();
    
    vector<int> distances(m_adjacency.size(), infinite);
    for (int i = 0; i < (int)distances.size(); ++i) {
      if (distances[i] == infinite) {
        distances[i] = 0;
        
        queue<int> work_queue;
        work_queue.push(i);
        
        while (!work_queue.empty()) {
          int v = work_queue.front();
          work_queue.pop();

          for (int w : m_adjacency[v]) {
            if (distances[w] == infinite) {
              work_queue.push(w);
              distances[w] = distances[v] + 1;
            }
            else if (abs(distances[w] - distances[v]) % 2 == 0) {
              return false;
            }
          }
        }
      }
    }

    return true;
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

private:
  const int m_vertex_count;
  const int m_edge_count;
  vector<vector<int>> m_adjacency;
};

int main() {
  int v, e;
  cin >> v >> e;

  Graph graph(v, e);
  if (graph.IsBipartite()) {
    cout << "YES";
  }
  else {
    cout << "NO";
  }
  
  return 0;
}

/*
4 2
1 2
3 2
*/

/*
3 3
1 2
2 3
3 1
*/
