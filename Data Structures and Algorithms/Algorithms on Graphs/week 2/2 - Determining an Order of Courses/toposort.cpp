#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;
using std::pair;

static int count = 0;

void dfs(vector<vector<int> > &adj, vector<int> &used, vector<int> &order, int x) {
  //write your code here
  used[x] = true;
  for (int y : adj[x]) {
    if (!used[y]) {
      dfs(adj, used, order, y);
    }
  }
  order[x] = count++;
}     

vector<int> toposort(vector<vector<int> > adj) {
  vector<int> used(adj.size(), 0);
  vector<int> order;
  //write your code here
  order.resize(adj.size());
  for (size_t x = 0; x < adj.size(); ++x) {
    if (!used[x]) {
      dfs(adj, used, order, x);
    }
  }

  vector<pair<int, int>> temp;
  temp.reserve(adj.size());
  for (size_t x = 0; x < order.size(); ++x) {
    temp.push_back({order[x], x});
  }

  std::sort(temp.rbegin(), temp.rend());
  for (size_t i = 0; i < order.size(); ++i) {
    order[i] = temp[i].second;
  }
  return order;
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
  vector<int> order = toposort(adj);
  for (size_t i = 0; i < order.size(); i++) {
    std::cout << order[i] + 1 << " ";
  }
}
