#include <iostream>
#include <vector>
#include <algorithm>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

using std::vector;
using std::ios_base;
using std::cin;
using std::cout;

struct Node
{
  Node* left = nullptr;
  Node* rigth = nullptr;
  int value = 0;
};

void InOrder(Node* root, vector<int>& result)
{
  if (root == nullptr) {
    return;
  }

  InOrder(root->left, result);
  result.push_back(root->value);
  InOrder(root->rigth, result);
}

void PreOrder(Node* root, vector<int>& result)
{
  if (root == nullptr) {
    return;
  }

  result.push_back(root->value);
  PreOrder(root->left, result);
  PreOrder(root->rigth, result);
}

void PostOrder(Node* root, vector<int>& result)
{
  if (root == nullptr) {
    return;
  }

  PostOrder(root->left, result);
  PostOrder(root->rigth, result);
  result.push_back(root->value);
}

class TreeOrders {
  int n;
  vector <int> key;
  vector <int> left;
  vector <int> right;
  Node* root = nullptr;

public:
  void read() {
    cin >> n;
    key.resize(n);
    left.resize(n);
    right.resize(n);

    vector<Node*> nodes(n, nullptr);
    for (int i = 0; i < n; i++) {
      cin >> key[i] >> left[i] >> right[i];

      if (nodes[i] == nullptr) {
        nodes[i] = new Node;
      }
      nodes[i]->value = key[i];

      if (left[i] != -1) {
        if (nodes[left[i]] == nullptr) {
          nodes[left[i]] = new Node;
        }
        nodes[i]->left = nodes[left[i]];
      }

      if (right[i] != -1) {
        if (nodes[right[i]] == nullptr) {
          nodes[right[i]] = new Node;
        }
        nodes[i]->rigth = nodes[right[i]];
      }
    }

    root = nodes[0];
  }


  vector <int> in_order() {
    vector<int> result;
    // Finish the implementation
    // You may need to add a new recursive method to do that
    InOrder(root, result);
    return result;
  }

  vector <int> pre_order() {
    vector<int> result;    
    // Finish the implementation
    // You may need to add a new recursive method to do that
    PreOrder(root, result);
    return result;
  }

  vector <int> post_order() {
    vector<int> result;
    // Finish the implementation
    // You may need to add a new recursive method to do that
    PostOrder(root, result);
    return result;
  }
};

void print(vector <int> a) {
  for (size_t i = 0; i < a.size(); i++) {
    if (i > 0) {
      cout << ' ';
    }
    cout << a[i];
  }
  cout << '\n';
}

int main_with_large_stack_space() {
  ios_base::sync_with_stdio(0);
  TreeOrders t;
  t.read();
  print(t.in_order());
  print(t.pre_order());
  print(t.post_order());
  return 0;
}

int main (int argc, char **argv)
{
#if defined(__unix__) || defined(__APPLE__)
  // Allow larger stack space
  const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
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
              std::cerr << "setrlimit returned result = " << result << std::endl;
          }
      }
  }
#endif

  return main_with_large_stack_space();
}
