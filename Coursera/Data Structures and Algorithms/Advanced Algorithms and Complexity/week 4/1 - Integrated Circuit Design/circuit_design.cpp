#include <algorithm>
#include <cmath>
#include <ios>
#include <iostream>
#include <vector>

struct Clause {
  int firstVar;
  int secondVar;
};

struct TwoSatisfiability {
  int numVars;
  std::vector<Clause> clauses;

  TwoSatisfiability(int n, int m)
    : numVars(n)
    , clauses(m)
  {}

  void firstDfs(
    int v,
    const std::vector<std::vector<int>>& graph,
    std::vector<bool>& visited,
    std::vector<int>& order)
  {
    visited[v] = true;

    for (int w : graph[v]) {
      if (!visited[w]) {
        firstDfs(w, graph, visited, order);
      }
    }

    order.push_back(v);
  }

  void secondDfs(
    int v,
    int counter,
    const std::vector<std::vector<int>>& graph,
    std::vector<bool>& visited,
    std::vector<int>& ids)
  {
    ids[v] = counter;
    visited[v] = true;

    for (int w : graph[v]) {
      if (!visited[w]) {
        secondDfs(w, counter, graph, visited, ids);
      }
    }
  }

  bool isSatisfiable(std::vector<int>& result) {
    // fill origGraph
    std::vector<std::vector<int>> origGraph(2 * numVars);
    for (const auto& clause : clauses) {
      int x, y, u, v;
      x = std::abs(clause.firstVar) - 1;
      y = std::abs(clause.secondVar) - 1;
      u = std::abs(clause.firstVar) + numVars - 1;
      v = std::abs(clause.secondVar) + numVars - 1;

      if (clause.firstVar < 0 && clause.secondVar < 0) {
        origGraph.at(x).push_back(v);
        origGraph.at(y).push_back(u);
      }
      else if (clause.firstVar > 0 && clause.secondVar > 0) {
        origGraph.at(u).push_back(y);
        origGraph.at(v).push_back(x);
      }
      else if (clause.firstVar < 0 && clause.secondVar > 0) {
        origGraph.at(x).push_back(y);
        origGraph.at(v).push_back(u);
      }
      else {
        origGraph.at(u).push_back(v);
        origGraph.at(y).push_back(x);
      }
    }
    
    // fill rOrigGraph
    std::vector<std::vector<int>> rOrigGraph(2 * numVars);
    for (int v = 0; v < (int)origGraph.size(); v++) {
      for (int w : origGraph[v]) {
        rOrigGraph[w].push_back(v);
      }
    }
    
    // first DFS
    std::vector<int> order;
    std::vector<bool> visited(2 * numVars, false);
    for (int v = 0; v < (int)rOrigGraph.size(); v++) {
      if (!visited[v]) {
        firstDfs(v, rOrigGraph, visited, order);
      }
    }
    std::reverse(order.begin(), order.end());
    
    // second DFS
    std::fill(visited.begin(), visited.end(), false);
    std::vector<int> ids(2 * numVars);
    int counter = 0;
    for (int v : order) {
      if (!visited[v]) {
        secondDfs(v, counter, origGraph, visited, ids);
        ++counter;
      }
    }
    
    // check is satisfiable
    for (int i = 1; i <= numVars; i++) {
      if (ids[i - 1] == ids[numVars + i - 1]) {
        return false;
      }
    }
    
    // prepare sccs
    std::vector<std::vector<int>> sccs(2 * numVars);
    for (int i = 0; i < 2 * numVars; i++) {
      sccs[ids[i]].push_back(i);
    }
    
    // get values
    for (const auto& scc : sccs) {
      for (int i : scc) {
        if (i < numVars) {
          if (result[i] == -1) {
            result[i] = 0;
          }
        }
        else {
          if (result[i - numVars] == -1) {
            result[i - numVars] = 1;
          }
        }
      }
    }
    
    return true;
  }
};

int main() {
  std::ios::sync_with_stdio(false);

  int n, m;
  std::cin >> n >> m;
  TwoSatisfiability twoSat(n, m);
  for (int i = 0; i < m; ++i) {
    std::cin >> twoSat.clauses[i].firstVar >> twoSat.clauses[i].secondVar;
  }

  std::vector<int> result(n, -1);
  if (twoSat.isSatisfiable(result)) {
    std::cout << "SATISFIABLE" << std::endl;
    for (int i = 1; i <= n; ++i) {
      if (result[i - 1]) {
        std::cout << -i;
      }
      else {
        std::cout << i;
      }
      if (i < n) {
        std::cout << " ";
      }
      else {
        std::cout << std::endl;
      }
    }
  }
  else {
    std::cout << "UNSATISFIABLE" << std::endl;
  }

  return 0;
}

/*
3 4
-1 2
-2 3
1 -3
3 2
*/
