#include <ios>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Edge {
  int from;
  int to;
};

struct ConvertHampathToSat {
  int numVertices;
  vector<Edge> edges;

  ConvertHampathToSat(int n, int m) :
    numVertices(n),
    edges(m)
  {}

  void printEquisatisfiableSatFormula() {
    ostringstream oss;
    
    int numberOfClauses = 0;
    int numberofVariables = numVertices * numVertices;

    // each node j must appear in the path
    for (int j = 1; j <= numVertices; ++j) {
      for (int i = 1; i <= numVertices; ++i) {
        oss << varnum(i, j) << ' ';
      }
      oss << "0\n";
      numberOfClauses++;
    }

    // no node j appears twice in the path.
    for (int j = 1; j <= numVertices; ++j) {
      for (int i = 1; i < numVertices; ++i) {
        for (int k = i + 1; k <= numVertices; ++k) {
          oss << -varnum(i, j) << ' ' << -varnum(k, j) << " 0\n";
          numberOfClauses++;
        }
      }
    }

    // every position i on the path must be occupied.
    for (int i = 1; i <= numVertices; ++i) {
      for (int j = 1; j <= numVertices; ++j) {
        oss << varnum(i, j) << ' ';
      }
      oss << "0\n";
      numberOfClauses++;
    }

    // no two nodes j and k occupy the same position in the path.
    for (int i = 1; i <= numVertices; ++i) {
      for (int j = 1; j < numVertices; ++j) {
        for (int k = j + 1; k <= numVertices; ++k) {
          oss << -varnum(i, j) << ' ' << -varnum(i, k) << " 0\n";
          numberOfClauses++;
        }
      }
    }

    set<pair<int, int>> edgesSet;
    for (const Edge& edge : edges) {
      edgesSet.insert({ edge.from, edge.to });
      edgesSet.insert({ edge.to, edge.from });
    }

    // nonadjacent nodes iand j cannot be adjacent in the path
    for (int i = 1; i <= numVertices; ++i) {
      for (int j = 1; j <= numVertices; ++j) {
        if (edgesSet.find({ i, j }) == edgesSet.end()) {
          for (int k = 1; k < numVertices; ++k) {
            oss << -varnum(k, i) << ' ' << -varnum(k + 1, j) << " 0\n";
            numberOfClauses++;
          }
        }
      }
    }

    cout << numberOfClauses << ' ' << numberofVariables << '\n';
    cout << oss.str();
  }
  
  int varnum(int i, int j) {
    return (i - 1) * numVertices + j;
  }
};

int main() {
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;
  ConvertHampathToSat converter(n, m);
  for (int i = 0; i < m; ++i) {
    cin >> converter.edges[i].from >> converter.edges[i].to;
  }

  converter.printEquisatisfiableSatFormula();

  return 0;
}
