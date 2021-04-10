#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

struct Node
{
  Node* left = nullptr;
  Node* rigth = nullptr;
  int value = 0;
};

struct FakeNode {
  int key;
  int left;
  int right;

  FakeNode() : key(0), left(-1), right(-1) {}
  FakeNode(int key_, int left_, int right_) : key(key_), left(left_), right(right_) {}
};

bool CheckBinaryTree(Node* root, int min, int max)
{
  if (root == nullptr) {
    return true;
  }

  if (root->value < min || root->value > max) {
    return false;
  }

  return CheckBinaryTree(root->left, min, root->value) && CheckBinaryTree(root->rigth, root->value, max);
}

bool IsBinarySearchTree(const vector<FakeNode>& tree) {
  // Implement correct algorithm here

  vector<Node*> nodes(tree.size(), nullptr);
  for (int i = 0; i < (int)tree.size(); ++i)
  {
    int value = tree[i].key;
    int left_idx = tree[i].left;
    int right_idx = tree[i].right;

    if (nodes[i] == nullptr) {
      nodes[i] = new Node;
    }
    nodes[i]->value = value;

    if (left_idx != -1) {
      if (nodes[left_idx] == nullptr) {
        nodes[left_idx] = new Node;
      }
      nodes[i]->left = nodes[left_idx];
    }

    if (right_idx != -1) {
      if (nodes[right_idx] == nullptr) {
        nodes[right_idx] = new Node;
      }
      nodes[i]->rigth = nodes[right_idx];
    }
  }

  Node* root = nodes.empty() ? nullptr : nodes[0];
  return CheckBinaryTree(root, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
}

int main() {
  int nodes;
  cin >> nodes;
  vector<FakeNode> tree;
  for (int i = 0; i < nodes; ++i) {
    int key, left, right;
    cin >> key >> left >> right;
    tree.push_back(FakeNode(key, left, right));
  }
  if (IsBinarySearchTree(tree)) {
    cout << "CORRECT" << endl;
  } else {
    cout << "INCORRECT" << endl;
  }
  return 0;
}
