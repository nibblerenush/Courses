#include <iostream>
#include <limits>
#include <vector>

struct Node
{
  Node* left = nullptr;
  Node* rigth = nullptr;
  int value = 0;
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

int main()
{
  int vertices_count;
  std::cin >> vertices_count;

  std::vector<Node*> nodes(vertices_count, nullptr);
  for (int i = 0; i < vertices_count; ++i)
  {
    int value;
    int left_idx;
    int right_idx;
    std::cin >> value >> left_idx >> right_idx;

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
  if (CheckBinaryTree(root, std::numeric_limits<int>::min(), std::numeric_limits<int>::max())) {
    std::cout << "CORRECT";
  }
  else {
    std::cout << "INCORRECT";
  }

  return 0;
}
