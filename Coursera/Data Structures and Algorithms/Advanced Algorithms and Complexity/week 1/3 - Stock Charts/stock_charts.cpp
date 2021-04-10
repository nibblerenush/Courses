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

int get_max_flow(FlowGraph& graph, int from, int to) {
  int flow = 0;
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

    // increase flow
    flow += x;

    // get residual network
    i = graph.size() - 1;
    while (path[i] != -1) {
      graph.add_flow(path[i], x);
      const FlowGraph::Edge& edge = graph.get_edge(path[i]);
      i = edge.from;
    }
  }

  return flow;
}

class StockCharts {
 public:
  void Solve() {
    vector<vector<int>> stock_data = ReadData();
    int result = MinCharts(stock_data);
    WriteResponse(result);
  }

 private:
  vector<vector<int>> ReadData() {
    int num_stocks, num_points;
    cin >> num_stocks >> num_points;
    vector<vector<int>> stock_data(num_stocks, vector<int>(num_points));
    for (int i = 0; i < num_stocks; ++i)
      for (int j = 0; j < num_points; ++j) {
        cin >> stock_data[i][j];
      }
    return stock_data;
  }

  void WriteResponse(int result) {
    cout << result << "\n";
  }

  int MinCharts(const vector<vector<int>>& stock_data) {
    // Replace this incorrect greedy algorithm with an
    // algorithm that correctly finds the minimum number
    // of charts on which we can put all the stock data
    // without intersections of graphs on one chart.
    
    int num_stocks = stock_data.size();
    // Vector of charts; each chart is a vector of indices of individual stocks.
    /*vector<vector<int>> charts;
    for (int i = 0; i < stock_data.size(); ++i) {
      bool added = false;
      for (auto& chart : charts) {
        bool can_add = true;
        for (int index : chart)
          if (!compare(stock_data[i], stock_data[index]) &&
              !compare(stock_data[index], stock_data[i])) {
            can_add = false;
            break;
          }
        if (can_add) {
          chart.push_back(i);
          added = true;
          break;
        }
      }
      if (!added) {
        charts.emplace_back(vector<int>{i});
      }
    }*/

    // Get initial graph
    vector<vector<int>> adj_list(num_stocks, vector<int>{});
    for (int i = 0; i < num_stocks; ++i) {
      for (int j = 0; j < num_stocks; ++j) {
        if (compare(stock_data[i], stock_data[j])) {
          adj_list[i].push_back(j);
        }
      }
    }

    // Get bipartite graph
    FlowGraph graph(2 * num_stocks + 2);
    for (int i = 0; i < num_stocks; ++i) {
      for (int j : adj_list[i]) {
        graph.add_edge(i + 1, num_stocks + j + 1, 1);
      }
    }

    // Add source
    for (int i = 0; i < num_stocks; ++i) {
      graph.add_edge(0, i + 1, 1);
    }

    // Add sink
    for (int j = 0; j < num_stocks; ++j) {
      graph.add_edge(num_stocks + j + 1, 2 * num_stocks + 1, 1);
    }

    // Get max flow
    int max_flow = get_max_flow(graph, 0, 2 * num_stocks + 1);

    // Get result
    return num_stocks - max_flow;
  }

  bool compare(const vector<int>& stock1, const vector<int>& stock2) {
    for (int i = 0; i < stock1.size(); ++i)
      if (stock1[i] >= stock2[i])
        return false;
    return true;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  StockCharts stock_charts;
  stock_charts.Solve();
  return 0;
}
