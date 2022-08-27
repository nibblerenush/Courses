#include <ios>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

struct Edge {
  int from;
  int to;
};

struct ConvertGSMNetworkProblemToSat {
  int numVertices;
  vector<Edge> edges;

  ConvertGSMNetworkProblemToSat(int n, int m) :
    numVertices(n),
    edges(m)
  {}

  void printEquisatisfiableSatFormula() {
    ostringstream oss;

    int numberOfClauses = 4 * numVertices + 3 * edges.size();
    int numberOfVariables = 3 * numVertices;
    oss << numberOfClauses << ' ' << numberOfVariables << '\n';
    
    for (int vertex = 1; vertex <= numVertices; ++vertex) {
      vector<int> vars = boolVarsNums(vertex);
      oss << vars[0] << ' ' << vars[1] << ' ' << vars[2] << " 0\n";
      oss << -vars[0] << ' ' << -vars[1] << " 0\n";
      oss << -vars[0] << ' ' << -vars[2] << " 0\n";
      oss << -vars[1] << ' ' << -vars[2] << " 0\n";
    }

    for (const Edge& edge : edges) {
      vector<int> fromVars = boolVarsNums(edge.from);
      vector<int> toVars = boolVarsNums(edge.to);
      oss << -fromVars[0] << ' ' << -toVars[0] << " 0\n";
      oss << -fromVars[1] << ' ' << -toVars[1] << " 0\n";
      oss << -fromVars[2] << ' ' << -toVars[2] << " 0\n";
    }

    cout << oss.str();
  }

  vector<int> boolVarsNums(int vertex) {
    return vector<int>{ 3 * vertex - 2, 3 * vertex - 1, 3 * vertex };
  }
};

int main() {
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  ConvertGSMNetworkProblemToSat converter(n, m);
  for (int i = 0; i < m; ++i) {
    cin >> converter.edges[i].from >> converter.edges[i].to;
  }

  converter.printEquisatisfiableSatFormula();

  return 0;
}
