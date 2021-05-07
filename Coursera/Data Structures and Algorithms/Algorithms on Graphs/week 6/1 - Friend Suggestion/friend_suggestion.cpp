#include <algorithm>
#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include <iostream>

using namespace std;

// External vector of size 2 - for forward and backward search.
// Internal 2-dimensional vector is vector of adjacency lists for each node.
typedef vector<vector<vector<int>>> Adj;

// Distances can grow out of int type
typedef long long Len;

// Vector of two priority queues - for forward and backward searches.
// Each priority queue stores the closest unprocessed node in its head.
typedef vector<priority_queue<pair<Len, int>,vector<pair<Len,int>>,greater<pair<Len,int>>>> Queue;

const Len MY_INFINITY = numeric_limits<Len>::max() / 4;

class Bidijkstra {
    // Number of nodes
    int n_;
    // Graph adj_[0] and cost_[0] correspond to the initial graph,
    // adj_[1] and cost_[1] correspond to the reversed graph.
    // Graphs are stored as vectors of adjacency lists corresponding
    // to nodes.
    // Adjacency list itself is stored in adj_, and the corresponding
    // edge costs are stored in cost_.
    Adj adj_;
    Adj cost_;
    // distance_[0] stores distances for the forward search,
    // and distance_[1] stores distances for the backward search.
    vector<vector<Len>> distance_;
    // Stores all the nodes visited either by forward or backward search.
    vector<int> workset_;
    // Stores a flag for each node which is True iff the node was visited
    // either by forward or backward search.
    vector<bool> visited_;

public:
    Bidijkstra(int n, Adj adj, Adj cost)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<Len>(n, MY_INFINITY)), visited_(n)
    { workset_.reserve(n); }

    // Initialize the data structures before new query,
    // clear the changes made by the previous query.
    void clear() {
        /*for (int i = 0; i < (int)workset_.size(); ++i) {
            int v = workset_[i];
            distance_[0][v] = distance_[1][v] = MY_INFINITY;
            visited_[v] = false;
        }
        workset_.clear();*/

        distance_.assign(2, vector<Len>(n_, MY_INFINITY));
        visited_.assign(n_, false);
        workset_.clear();
    }

    // Processes visit of either forward or backward search 
    // (determined by value of side), to node v trying to
    // relax the current distance by dist.
    void visit(Queue& q, int side, int v, Len dist) {
        // Implement this method yourself
        
        const int size = adj_[side][v].size();
        for (int i = 0; i < size; ++i) {
            const int w = adj_[side][v][i];
            const int cost = cost_[side][v][i];
            
            if (distance_[side][w] > dist + cost) {
                distance_[side][w] = dist + cost;
                q[side].push({ distance_[side][w], w });
            }
        }
        
        visited_[v] = true;
        workset_.push_back(v);
    }

    // Returns the distance from s to t in the graph.
    Len query(int s, int t) {
        if (s == t) {
            return 0;
        }

        clear();
        Queue q(2);
        visit(q, 0, s, 0);
        visit(q, 1, t, 0);
        // Implement the rest of the algorithm yourself

        distance_[0][s] = 0;
        distance_[1][t] = 0;

        while (!q[0].empty() && !q[1].empty()) {
            pair<Len, int> item0 = q[0].top();
            q[0].pop();

            if (visited_[item0.second]) {
                Len distance = MY_INFINITY;
                for (int v : workset_) {
                    distance = min(distance, distance_[0][v] + distance_[1][v]);
                }
                return distance;
            }
            visit(q, 0, item0.second, item0.first);
            
            pair<Len, int> item1 = q[1].top();
            q[1].pop();

            if (visited_[item1.second]) { 
                Len distance = MY_INFINITY;
                for (int v : workset_) {
                    distance = min(distance, distance_[0][v] + distance_[1][v]);
                }
                return distance;
            }
            visit(q, 1, item1.second, item1.first);
        }

        return -1;
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    Adj adj(2, vector<vector<int>>(n));
    Adj cost(2, vector<vector<int>>(n));
    for (int i=0; i<m; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        adj[0][u-1].push_back(v-1);
        cost[0][u-1].push_back(c);
        adj[1][v-1].push_back(u-1);
        cost[1][v-1].push_back(c);
    }

    Bidijkstra bidij(n, adj, cost);

    int t;
    cin >> t;
    for (int i=0; i<t; ++i) {
        int u, v;
        cin >> u >> v;
        printf("%lld\n", bidij.query(u-1, v-1));
    }

    return 0;
}
