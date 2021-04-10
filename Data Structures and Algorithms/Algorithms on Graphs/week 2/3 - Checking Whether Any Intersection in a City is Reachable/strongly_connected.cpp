#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;
using std::pair;

static int counter = 0;

vector<vector<int>> get_reverse_graph(const vector<vector<int>>& adj) {
  vector<vector<int>> result(adj.size(), vector<int>{});
  for (size_t v = 0; v < adj.size(); ++v) {
    for (int w : adj[v]) {
      result[w].push_back(v);
    }
  }
  return result;
}

void explore(const vector<vector<int>>& adj, vector<bool>& visited, vector<int>& postorder, int v) {
  visited[v] = true;
  for (int w : adj[v]) {
    if (!visited[w]) {
      explore(adj, visited, postorder, w);
    }
  }
  postorder[v] = counter++;
}

int number_of_strongly_connected_components(vector<vector<int>>& adj) {
  int result = 0;
  //write your code here

  auto reverse_graph = get_reverse_graph(adj);
  vector<bool> visited(reverse_graph.size(), false);
  vector<int> postorder(reverse_graph.size(), 0);

  // dfs
  for (size_t v = 0; v < reverse_graph.size(); ++v) {
    if (!visited[v]) {
      explore(reverse_graph, visited, postorder, v);
    }
  }

  vector<pair<int, int>> temp;
  temp.reserve(reverse_graph.size());
  for (size_t v = 0; v < postorder.size(); ++v) {
    temp.push_back({postorder[v], v});
  }
  std::sort(temp.rbegin(), temp.rend());

  visited.assign(reverse_graph.size(), false);
  for (const pair<int, int>& item : temp) {
    if (!visited[item.second]) {
      explore(adj, visited, postorder, item.second);
      result++;
    }
  }

  return result;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
  }
  std::cout << number_of_strongly_connected_components(adj);
}
