#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

static const long long infinite = numeric_limits<long long>::max();

struct OutEdge {
  int toVertex = 0;
  long long cost = 0;
};

struct InEdge {
  int fromVertex = 0;
  long long cost = 0;
};

struct PreprocessEntry {
  int rank = 0;
  int vertex = 0;
};

struct Shortcut {
  int fromVertex = 0;
  int toVertex = 0;
  long long distance = 0;
};

bool operator > (const PreprocessEntry& lh, const PreprocessEntry& rh) {
  if (lh.rank != rh.rank) {
    return lh.rank > rh.rank;
  }
  else {
    return lh.vertex > rh.vertex;
  }
}

long long dijkstra_for_shortcut(
  int s,
  int t,
  long long simpleDistance,
  const vector<vector<OutEdge>>& outEdges)
{
  vector<long long> distances(outEdges.size(), infinite);
  distances[s] = 0;
  
  // pair<int, int> - distance, vertex
  using dist_v_t = pair<long long, int>;
  priority_queue<dist_v_t, vector<dist_v_t>, greater<dist_v_t>> queue;
  queue.push({ 0, s });

  const int max_hops = 5;
  int cur_hop = 1;
  
  while (!queue.empty()) {
    if (cur_hop > max_hops) {
      return -1;
    }

    dist_v_t item = queue.top();
    queue.pop();
    
    if (item.first > simpleDistance) {
      return -1;
    }
    
    for (const OutEdge& outEdge : outEdges[item.second]) {
      int w = outEdge.toVertex;
      long long cost = outEdge.cost;

      if (distances[w] > item.first + cost) {
        distances[w] = item.first + cost;
        queue.push({ distances[w], w });
      }
    }

    cur_hop += 1;
  }
  
  return distances[t] != infinite ? distances[t] : -1;
}

vector<Shortcut> get_shortcuts(
  int v,
  const vector<vector<OutEdge>>& outEdges,
  const vector<vector<InEdge>>& inEdges)
{
  vector<Shortcut> shortcuts;
  
  for (const InEdge& inEdge : inEdges[v]) {
    for (const OutEdge& outEdge : outEdges[v]) {
      int s = inEdge.fromVertex;
      int t = outEdge.toVertex;
      long long simpleDistance = inEdge.cost + outEdge.cost;
      long long dijkstra_distance = dijkstra_for_shortcut(s, t, simpleDistance, outEdges);

      if (dijkstra_distance != -1) {
        shortcuts.push_back({ s, t, dijkstra_distance });
      }
    }
  }
  
  return shortcuts;
}

void preprocess(vector<vector<OutEdge>>& outEdges, vector<vector<InEdge>>& inEdges) {
  const int n = outEdges.size();
  vector<int> ranks(n);
  priority_queue<PreprocessEntry, vector<PreprocessEntry>, greater<PreprocessEntry>> work_queue;
  
  for (int i = 0; i < n; ++i) {
    vector<Shortcut> shortcuts = get_shortcuts(i, outEdges, inEdges);
    int rank = shortcuts.size() - outEdges[i].size() - inEdges[i].size();
    work_queue.push(PreprocessEntry{ rank, i });
    ranks[i] = rank;
  }

  while (!work_queue.empty()) {
    PreprocessEntry entry = work_queue.top();
    work_queue.pop();

    vector<Shortcut> shortcuts = get_shortcuts(entry.vertex, outEdges, inEdges);
    int rank = shortcuts.size() - outEdges[entry.vertex].size() - inEdges[entry.vertex].size();
    ranks[entry.vertex] = rank;

    if (work_queue.empty() || rank <= work_queue.top().rank) {
      for (const Shortcut& shortcut : shortcuts) {
        int s = shortcut.fromVertex;
        int t = shortcut.toVertex;
        long long cost = shortcut.distance;

        if (ranks[s] < ranks[t]) {
          outEdges[s].push_back(OutEdge{ t, cost });
          inEdges[t].push_back(InEdge{ s, cost });
        }
      }
    }
    else {
      work_queue.push(PreprocessEntry{ rank, entry.vertex });
    }
  }

  /*for (int i = 0; i < (int)outEdges.size(); ++i) {
    for (const OutEdge& outEdge : outEdges[i]) {
      int u = i;
      int v = outEdge.toVertex;
      if (ranks[u] > ranks[v]) {

      }
    }
  }*/
}

long long ComputeDistance(
  int s,
  int t,
  const vector<vector<OutEdge>>& outEdges,
  const vector<vector<InEdge>>& inEdges)
{
  const int n = outEdges.size();
  long long estimate = infinite;

  vector<long long> dist(n, infinite);
  dist[s] = 0;

  vector<long long> dist_r(n, infinite);
  dist_r[t] = 0;

  vector<bool> proc(n, false);
  proc[s] = true;

  vector<bool> proc_r(n, false);
  proc_r[t] = 0;

  using dist_v_t = pair<long long, int>;

  priority_queue<dist_v_t, vector<dist_v_t>, greater<dist_v_t>> work_queue;
  work_queue.push({ 0, s });

  priority_queue<dist_v_t, vector<dist_v_t>, greater<dist_v_t>> work_queue_r;
  work_queue_r.push({ 0, t });

  while (!work_queue.empty() || !work_queue_r.empty()) {
    if (!work_queue.empty()) {
      dist_v_t item = work_queue.top();
      work_queue.pop();

      int v = item.second;

      if (dist[v] < estimate) {
        for (const OutEdge& outEdge : outEdges[v]) {
          int w = outEdge.toVertex;
          long long cost = outEdge.cost;

          if (dist[w] > item.first + cost) {
            dist[w] = item.first + cost;
            work_queue.push({ dist[w], w });
          }
        }

        proc[v] = true;
      }

      if (proc_r[v] && dist[v] + dist_r[v] < estimate) {
        estimate = dist[v] + dist_r[v];
      }
    }

    if (!work_queue_r.empty()) {
      dist_v_t item = work_queue_r.top();
      work_queue_r.pop();

      int v = item.second;

      if (dist_r[v] < estimate) {
        /*for (const OutEdge& outEdge : outEdges_r[v]) {
          int w = outEdge.toVertex;
          long long cost = outEdge.cost;

          if (dist_r[w] > item.first + cost) {
            dist_r[w] = item.first + cost;
            work_queue_r.push({ dist_r[w], w });
          }
        }*/

        for (const InEdge& inEdge : inEdges[v]) {
          int w = inEdge.fromVertex;
          long long cost = inEdge.cost;

          if (dist_r[w] > item.first + cost) {
            dist_r[w] = item.first + cost;
            work_queue_r.push({ dist_r[w], w });
          }
        }

        proc_r[v] = true;
      }

      if (proc[v] && dist[v] + dist_r[v] < estimate) {
        estimate = dist[v] + dist_r[v];
      }
    }
  }

  return estimate != infinite ? estimate : -1;
}

int main() {
  int n, m;
  cin >> n >> m;
  
  vector<vector<OutEdge>> outEdges(n);
  vector<vector<InEdge>> inEdges(n);
  //vector<vector<int>> costs(n);
  
  for (int i = 0; i < m; ++i) {
    int u, v, l;
    cin >> u >> v >> l;
    outEdges[u - 1].push_back(OutEdge{ v - 1, l });
    inEdges[v - 1].push_back(InEdge{ u - 1, l });
    //costs[u - 1].push_back(l);
  }

  preprocess(outEdges, inEdges);

  /*vector<vector<OutEdge>> outEdges_r(n);
  for (int i = 0; i < n; ++i) {
    for (const OutEdge& outEdge : outEdges[i]) {
      int fromVertex = i;
      int toVertex = outEdge.toVertex;
      long long cost = outEdge.cost;
      outEdges_r[toVertex].push_back(OutEdge{ fromVertex, cost });
    }
  }*/

  cout << "Ready" << endl;

  int q;
  cin >> q;
  vector<long long> result(q);

  for (int i = 0; i < q; ++i) {
    int s, t;
    cin >> s >> t;
    result[i] = ComputeDistance(s - 1, t - 1, outEdges, inEdges);
  }

  for (int num : result) {
    cout << num << '\n';
  }

  return 0;
}
