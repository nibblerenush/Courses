#include <iostream>
#include <vector>

struct Node
{
  Node* left = nullptr;
  Node* rigth = nullptr;
  int value = 0;
};

void InOrder(Node* root)
{
  if (root == nullptr) {
    return;
  }

  InOrder(root->left);
  std::cout << root->value << ' ';
  InOrder(root->rigth);
}

void PreOrder(Node* root)
{
  if (root == nullptr) {
    return;
  }

  std::cout << root->value << ' ';
  PreOrder(root->left);
  PreOrder(root->rigth);
}

void PostOrder(Node* root)
{
  if (root == nullptr) {
    return;
  }

  PostOrder(root->left);
  PostOrder(root->rigth);
  std::cout << root->value << ' ';
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

  InOrder(nodes[0]);
  std::cout << std::endl;
  PreOrder(nodes[0]);
  std::cout << std::endl;
  PostOrder(nodes[0]);

  return 0;
}
