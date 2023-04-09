#include <iostream>
#include <vector>

struct Vertex {
  int weight = 0;
  std::vector <int> children;
};

typedef std::vector<Vertex> Graph;
typedef std::vector<int> Sum;

Graph ReadTree() {
  int vertices_count;
  std::cin >> vertices_count;

  Graph tree(vertices_count);

  for (int i = 0; i < vertices_count; ++i) {
    std::cin >> tree[i].weight;
  }
  
  for (int i = 1; i < vertices_count; ++i) {
    int from, to;
    std::cin >> from >> to;
    tree[from - 1].children.push_back(to - 1);
    tree[to - 1].children.push_back(from - 1);
  }

  return tree;
}

int dfs(const Graph& tree, int vertex, int parent, Sum& sum) {
  // This is a template function for processing a tree using depth-first search.
  // Write your code here.
  // You may need to add more parameters to this function for child processing.
  
  // check if already handled
  if (sum[vertex] == -1) {
    
    // check if leaf
    if (tree[vertex].children.size() == 1 && tree[vertex].children.back() == parent) {
      sum[vertex] = tree[vertex].weight;
    }
    // main realization
    else {
      int m1 = tree[vertex].weight;
      for (int child : tree[vertex].children) {
        if (child != parent) {
          for (int grandson : tree[child].children) {
            if (grandson != vertex) {
              m1 += dfs(tree, grandson, child, sum);
            }
          }
        }
      }

      int m0 = 0;
      for (int child : tree[vertex].children) {
        if (child != parent) {
          m0 += dfs(tree, child, vertex, sum);
        }
      }

      sum[vertex] = std::max(m1, m0);
    }
  }
  
  return sum[vertex];
}

int MaxWeightIndependentTreeSubset(const Graph& tree) {
  size_t size = tree.size();
  if (size == 0) {
    return 0;
  }
  
  Sum sum(size, -1);
  int result = dfs(tree, 0, -1, sum);

  // You must decide what to return.
  return result;
}

int main() {
  // This code is here to increase the stack size to avoid stack overflow
  // in depth-first search.
  /*const rlim_t kStackSize = 64L * 1024L * 1024L;  // min stack size = 64 Mb
  struct rlimit rl;
  int result;
  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0)
  {
      if (rl.rlim_cur < kStackSize)
      {
          rl.rlim_cur = kStackSize;
          result = setrlimit(RLIMIT_STACK, &rl);
          if (result != 0)
          {
              fprintf(stderr, "setrlimit returned result = %d\n", result);
          }
      }
  }*/

  // Here begins the solution
  Graph tree = ReadTree();
  int weight = MaxWeightIndependentTreeSubset(tree);
  std::cout << weight << std::endl;
  return 0;
}

/*
5
1 5 3 7 5
5 4
2 3
4 2
1 2
*/
