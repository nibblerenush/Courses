#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

using std::cin;
using std::cout;
using std::queue;
using std::vector;

/* This class implements a bit unusual scheme for storing edges of the graph,
 * in order to retrieve the backward edge for a given edge quickly. */
class FlowGraph {
public:
  struct Edge {
    int from, to, capacity, flow;
  };

private:
  /* List of all - forward and backward - edges */
  vector<Edge> edges;

  /* These adjacency lists store only indices of edges in the edges list */
  vector<vector<size_t> > graph;

public:
  explicit FlowGraph(size_t n) : graph(n) {}

  void add_edge(int from, int to, int capacity) {
    /* Note that we first append a forward edge and then a backward edge,
     * so all forward edges are stored at even indices (starting from 0),
     * whereas backward edges are stored at odd indices in the list edges */
    Edge forward_edge = { from, to, capacity, 0 };
    Edge backward_edge = { to, from, 0, 0 };
    graph[from].push_back(edges.size());
    edges.push_back(forward_edge);
    graph[to].push_back(edges.size());
    edges.push_back(backward_edge);
  }

  size_t size() const {
    return graph.size();
  }

  const vector<size_t>& get_ids(int from) const {
    return graph[from];
  }

  const Edge& get_edge(size_t id) const {
    return edges[id];
  }

  void add_flow(size_t id, int flow) {
    /* To get a backward edge for a true forward edge (i.e id is even), we should get id + 1
     * due to the described above scheme. On the other hand, when we have to get a "backward"
     * edge for a backward edge (i.e. get a forward edge for backward - id is odd), id - 1
     * should be taken.
     *
     * It turns out that id ^ 1 works for both cases. Think this through! */
    edges[id].flow += flow;
    edges[id ^ 1].flow -= flow;
  }
};

bool bfs(FlowGraph& graph, vector<int>& path) {
  vector<bool> visited(graph.size(), false);
  visited[0] = true;

  queue<int> work_queue;
  work_queue.push(0);

  while (!work_queue.empty()) {
    int v = work_queue.front();
    work_queue.pop();

    if (v == graph.size() - 1) {
      return true;
    }

    for (int edge_id : graph.get_ids(v)) {
      const FlowGraph::Edge& edge = graph.get_edge(edge_id);

      if (!visited[edge.to] && (edge.capacity - edge.flow) > 0) {
        path[edge.to] = edge_id;
        work_queue.push(edge.to);
        visited[edge.to] = true;
      }
    }
  }

  return false;
}

void residual_network(FlowGraph& graph, int from, int to) {
  vector<int> path(graph.size(), -1);

  while (true) {
    // get augmenting path
    bool is_path_exists = bfs(graph, path);
    if (!is_path_exists) {
      break;
    }

    // get minimum capacity on augmenting path
    int x = std::numeric_limits<int>::max();
    int i = graph.size() - 1;
    while (path[i] != -1) {
      const FlowGraph::Edge& edge = graph.get_edge(path[i]);
      x = std::min(x, edge.capacity - edge.flow);
      i = edge.from;
    }

    // get residual network
    i = graph.size() - 1;
    while (path[i] != -1) {
      graph.add_flow(path[i], x);
      const FlowGraph::Edge& edge = graph.get_edge(path[i]);
      i = edge.from;
    }
  }
}

class MaxMatching {
 public:
  void Solve() {
    vector<vector<bool>> adj_matrix = ReadData();
    vector<int> matching = FindMatching(adj_matrix);
    WriteResponse(matching);
  }

 private:
  vector<vector<bool>> ReadData() {
    int num_left, num_right;
    cin >> num_left >> num_right;
    vector<vector<bool>> adj_matrix(num_left, vector<bool>(num_right));
    for (int i = 0; i < num_left; ++i)
      for (int j = 0; j < num_right; ++j) {
        int bit;
        cin >> bit;
        adj_matrix[i][j] = (bit == 1);
      }
    return adj_matrix;
  }

  void WriteResponse(const vector<int>& matching) {
    for (int i = 0; i < matching.size(); ++i) {
      if (i > 0)
        cout << " ";
      if (matching[i] == -1)
        cout << "-1";
      else
        cout << (matching[i] + 1);
    }
    cout << "\n";
  }

  vector<int> FindMatching(const vector<vector<bool>>& adj_matrix) {
    // Replace this code with an algorithm that finds the maximum
    // matching correctly in all cases.

    /*int num_left = adj_matrix.size();
    int num_right = adj_matrix[0].size();
    vector<int> matching(num_left, -1);
    vector<bool> busy_right(num_right, false);
    for (int i = 0; i < num_left; ++i)
      for (int j = 0; j < num_right; ++j)
        if (matching[i] == -1 && !busy_right[j] && adj_matrix[i][j]) {
          matching[i] = j;
          busy_right[j] = true;
        }
    return matching;*/

    int num_left = adj_matrix.size();
    int num_right = adj_matrix[0].size();

    // Construct graph
    FlowGraph graph(num_left + num_right + 2);
    for (int i = 0; i < num_left; ++i) {
      for (int j = 0; j < num_right; ++j) {
        if (adj_matrix[i][j] == 1) {
          graph.add_edge(i + 1, num_left + j + 1, 1);
        }
      }
    }

    // Add source
    for (int i = 0; i < num_left; ++i) {
      graph.add_edge(0, i + 1, 1);
    }

    // Add sink
    for (int j = 0; j < num_right; ++j) {
      graph.add_edge(num_left + j + 1, num_left + num_right + 1, 1);
    }

    // Get residual network
    residual_network(graph, 0, num_left + num_right + 1);
    
    // Get matching
    vector<int> matching(num_left, -1);
    for (int edge_id : graph.get_ids(num_left + num_right + 1)) {
      const FlowGraph::Edge& edge = graph.get_edge(edge_id);
      if (edge.flow < 0) {
        for (int child_edge_id : graph.get_ids(edge.to)) {
          const FlowGraph::Edge& child_edge = graph.get_edge(child_edge_id);
          if (child_edge.flow < 0) {
            matching[child_edge.to - 1] = child_edge.from - num_left - 1;
            break;
          }
        }
      }
    }

    return matching;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  MaxMatching max_matching;
  max_matching.Solve();
  return 0;
}
