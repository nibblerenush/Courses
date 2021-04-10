#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <cmath>
#include <tuple>

using std::vector;
using std::pair;

class DisjoinSet {
public:
  DisjoinSet(std::size_t size) : m_rank(size), m_parent(size), m_sets_count(0)
  {}

  void MakeSet(int i) {
    m_parent[i] = i;
    m_rank[i] = 0;
    m_sets_count++;
  }

  int Find(int i) {
    if (i != m_parent[i]) {
      m_parent[i] = Find(m_parent[i]);
    }
    return m_parent[i];
  }

  void Union(int i, int j) {
    int i_id = Find(i);
    int j_id = Find(j);

    if (i_id == j_id) {
      return;
    }

    if (m_rank[i_id] > m_rank[j_id]) {
      m_parent[j_id] = i_id;
    }
    else {
      m_parent[i_id] = j_id;
      if (m_rank[i_id] == m_rank[j_id]) {
        m_rank[j_id] += 1;
      }
    }
    m_sets_count--;
  }

  int GetSetsCount() const {
    return m_sets_count;
  }

private:
  std::vector<int> m_rank;
  std::vector<int> m_parent;
  int m_sets_count;
};

struct Point {
  int x;
  int y;
};

struct Edge {
  double weight;
  int from;
  int to;
};

bool operator<(const Edge& lhs, const Edge& rhs) {
  return std::tie(lhs.weight, lhs.from, lhs.to) < std::tie(rhs.weight, rhs.from, rhs.to);
}

double get_distance(Point p1, Point p2) {
  int x_sqr = (p1.x - p2.x) * (p1.x - p2.x);
  int y_sqr = (p1.y - p2.y) * (p1.y - p2.y);
  return std::sqrt(x_sqr + y_sqr);
}

double clustering(vector<int> x, vector<int> y, int k) {
  //write your code here
  const int size = x.size();

  DisjoinSet disjoin_set(size);
  std::vector<Point> points;
  for (int i = 0; i < size; ++i) {
    points.push_back({ x[i], y[i] });
    disjoin_set.MakeSet(i);
  }

  std::vector<Edge> edges;
  for (int i = 0; i < size; ++i) {
    for (int j = i + 1; j < size; ++j) {
      edges.push_back({ get_distance(points[i], points[j]), i, j });
    }
  }
  std::sort(edges.begin(), edges.end());

  for (const auto& edge : edges) {
    if (disjoin_set.Find(edge.from) != disjoin_set.Find(edge.to)) {
      disjoin_set.Union(edge.from, edge.to);
    }

    if (disjoin_set.GetSetsCount() == k - 1) {
      return edge.weight;
    }
  }

  return -1.;
}

int main() {
  size_t n;
  int k;
  std::cin >> n;
  vector<int> x(n), y(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> x[i] >> y[i];
  }
  std::cin >> k;
  std::cout << std::setprecision(10) << clustering(x, y, k) << std::endl;
}
