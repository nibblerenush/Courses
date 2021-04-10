#include <algorithm>
#include <iostream>
#include <vector>

struct Node
{
  Node* parent = nullptr;
  std::vector<Node*> children;
};

int Height(Node* root)
{
  int height = 1;
  for (Node* node : root->children)
  {
    height = std::max(height, 1 + Height(node));
  }
  return height;
}

int main()
{
  int number_of_nodes;
  std::cin >> number_of_nodes;
  
  std::vector<Node*> nodes(number_of_nodes, nullptr);
  Node* root = nullptr;
  for (int i = 0; i < number_of_nodes; ++i)
  {
    if (nodes[i] == nullptr) {
      nodes[i] = new Node;
    }
    
    int parent;
    std::cin >> parent;

    if (parent == -1) {
      root = nodes[i];
      continue;
    }
    
    if (nodes[parent] == nullptr) {
      nodes[parent] = new Node;
    }

    nodes[i]->parent = nodes[parent];
    nodes[parent]->children.push_back(nodes[i]);
  }
  std::cout << Height(root) << std::endl;
  return 0;
}
