#include <iostream>
#include <string>

struct Node
{
  Node* parent = nullptr;
  Node* left = nullptr;
  Node* right = nullptr;
  int sum = 1;
  char letter = 0;

  ~Node() {
    delete left;
    delete right;
  }
};

class Rope
{
public:
  Rope(const std::string& word)
  {
    for (const auto& letter : word)
    {
      if (m_root == nullptr) {
        m_root = new Node;
        m_root->letter = letter;
        continue;
      }
      
      Node* new_node = new Node;
      new_node->letter = letter;
      
      Node* prev_node = FindRight(m_root);
      prev_node->right = new_node;
      new_node->parent = prev_node;
      
      m_root = splay(new_node);
    }
  }

  Node* Find(int key) {
    m_root = FindPrivate(key, m_root);
    return m_root;
  }

  void CutAndPaste(int i, int j, int k)
  {
    Node* right_part = Find(j);
    Node* left_part = Find(i);
    
    left_part = splay(left_part);
    Node* left_part_left = left_part->left;

    if (left_part_left) {
      left_part->sum -= left_part_left->sum;
      left_part_left->parent = nullptr;
      left_part->left = nullptr;
    }

    right_part = splay(right_part);
    Node* right_part_right = right_part->right;

    if (right_part_right) {
      right_part->sum -= right_part_right->sum;
      right_part_right->parent = nullptr;
      right_part->right = nullptr;
    }
    
    Node* merge_node = nullptr;
    if (left_part_left) {
      merge_node = FindRight(left_part_left);
      if (merge_node) {
        merge_node->right = right_part_right;
        if (right_part_right) {
          right_part_right->parent = merge_node;
          merge_node->sum += right_part_right->sum;
        }
        merge_node = splay(merge_node);
      }
    }
    else {
      merge_node = right_part_right;
    }

    if (k != 0)
    {
      Node* after_node = FindPrivate(k, merge_node);
      after_node = splay(after_node);
      
      Node* after_node_right = nullptr;
      if (after_node) {
        after_node_right = after_node->right;
        after_node->right = right_part;
      }

      right_part->parent = after_node;
      right_part = splay(right_part);

      Node* best_right = FindRight(right_part);
      if (best_right) {
        best_right->right = after_node_right;
        if (after_node_right) {
          after_node_right->parent = best_right;
          best_right->sum += after_node_right->sum;
        }
        m_root = splay(best_right);
      }
    }
    else
    {
      Node* best_right = FindRight(right_part);
      if (best_right) {
        best_right->right = merge_node;
        if (merge_node) {
          merge_node->parent = best_right;
          best_right->sum += merge_node->sum;
        }
        m_root = splay(best_right);
      }
    }
  }

  ~Rope() {
    delete m_root;
  }

private:
  Node* m_root = nullptr;

  Node* FindRight(Node* root)
  {
    Node* cur_node = root;
    Node* prev_node = nullptr;

    while (cur_node != nullptr) {
      prev_node = cur_node;
      cur_node = cur_node->right;
    }

    return prev_node;
  }

  Node* FindPrivate(int key, Node* root)
  {
    Node* cur_node = root;
    Node* prev_node = nullptr;

    while (key != 0 && cur_node)
    {
      prev_node = cur_node;
      int left_sum = cur_node->left ? cur_node->left->sum : 0;

      if (key > left_sum) {
        key = key - (left_sum + 1);
        cur_node = cur_node->right;
      }
      else {
        cur_node = cur_node->left;
      }
    }

    root = splay(prev_node);
    return root;
  }

  Node* splay(Node* node)
  {
    if (node == nullptr || node->parent == nullptr) {
      return node;
    }

    Node* parent = node->parent;

    // Zig, ZigZig, ZigZag
    if (parent->left == node)
    {
      // Zig
      if (parent->parent == nullptr) {
        return splay(Zig(node));
      }
      // ZigZig
      if (parent->parent->left == parent) {
        return splay(ZigZig(node));
      }
      // ZigZag
      else {
        return splay(ZigZag(node));
      }
    }
    // Zag, ZagZag, ZagZig
    else
    {
      // Zag
      if (parent->parent == nullptr) {
        return splay(Zag(node));
      }
      // ZagZag
      if (parent->parent->right == parent) {
        return splay(ZagZag(node));
      }
      // ZagZig
      else {
        return splay(ZagZig(node));
      }
    }
  }

  void FixPtrToNode(Node* grand_parent, Node* node)
  {
    if (grand_parent && grand_parent->parent)
    {
      if (grand_parent->parent->left == grand_parent) {
        grand_parent->parent->left = node;
      }
      else if (grand_parent->parent->right) {
        grand_parent->parent->right = node;
      }
    }
  }

  void UpdateSum(Node* node)
  {
    node->sum = 1;
    if (node->left) { node->sum += node->left->sum; }
    if (node->right) { node->sum += node->right->sum; }
  }

  Node* Zig(Node* node)
  {
    Node* parent = node->parent;
    Node* right_son = node->right;

    node->parent = parent->parent; // nullptr
    // node->left - not changed
    node->right = parent;

    parent->parent = node;
    parent->left = right_son;
    // parent->right - not changed

    if (right_son != nullptr) {
      right_son->parent = parent;
    }

    UpdateSum(parent);
    UpdateSum(node);

    return node;
  }

  Node* ZigZig(Node* node)
  {
    Node* parent = node->parent;
    Node* right_son = node->right;

    Node* grand_parent = parent->parent;
    Node* parent_right_son = parent->right;

    FixPtrToNode(grand_parent, node);

    node->parent = grand_parent->parent;
    // node->left - not changed
    node->right = parent;

    parent->parent = node;
    parent->left = right_son;
    parent->right = grand_parent;

    if (right_son != nullptr) {
      right_son->parent = parent;
    }

    grand_parent->parent = parent;
    grand_parent->left = parent_right_son;
    // grand_parent->right - not changed

    if (parent_right_son != nullptr) {
      parent_right_son->parent = grand_parent;
    }

    UpdateSum(grand_parent);
    UpdateSum(parent);
    UpdateSum(node);

    return node;
  }

  Node* ZigZag(Node* node)
  {
    Node* parent = node->parent;
    Node* left_son = node->left;
    Node* right_son = node->right;

    Node* grand_parent = parent->parent;

    FixPtrToNode(grand_parent, node);

    node->parent = grand_parent->parent;
    node->left = grand_parent;
    node->right = parent;

    grand_parent->parent = node;
    // grand_parent->left - not changed
    grand_parent->right = left_son;

    if (left_son != nullptr) {
      left_son->parent = grand_parent;
    }

    parent->parent = node;
    parent->left = right_son;
    // parent->right - not changed

    if (right_son != nullptr) {
      right_son->parent = parent;
    }

    UpdateSum(grand_parent);
    UpdateSum(parent);
    UpdateSum(node);

    return node;
  }

  Node* Zag(Node* node)
  {
    Node* parent = node->parent;
    Node* left_son = node->left;

    node->parent = parent->parent; // nullptr
    node->left = parent;
    // node->right - not changed;

    parent->parent = node;
    // parent->left - not changed;
    parent->right = left_son;

    if (left_son != nullptr) {
      left_son->parent = parent;
    }

    UpdateSum(parent);
    UpdateSum(node);

    return node;
  }

  Node* ZagZag(Node* node)
  {
    Node* parent = node->parent;
    Node* left_son = node->left;

    Node* grand_parent = parent->parent;
    Node* parent_left_son = parent->left;

    FixPtrToNode(grand_parent, node);

    node->parent = grand_parent->parent;
    node->left = parent;
    // node->right - not changed;

    parent->parent = node;
    parent->right = left_son;
    parent->left = grand_parent;

    if (left_son != nullptr) {
      left_son->parent = parent;
    }

    grand_parent->parent = parent;
    grand_parent->right = parent_left_son;
    // grand_parent->left - not changed

    if (parent_left_son != nullptr) {
      parent_left_son->parent = grand_parent;
    }

    UpdateSum(grand_parent);
    UpdateSum(parent);
    UpdateSum(node);

    return node;
  }

  Node* ZagZig(Node* node)
  {
    Node* parent = node->parent;
    Node* left_son = node->left;
    Node* right_son = node->right;

    Node* grand_parent = parent->parent;

    FixPtrToNode(grand_parent, node);

    node->parent = grand_parent->parent;
    node->left = parent;
    node->right = grand_parent;

    parent->parent = node;
    // parent->left - not changed
    parent->right = left_son;

    if (left_son != nullptr) {
      left_son->parent = parent;
    }

    grand_parent->parent = node;
    grand_parent->left = right_son;
    // grand_parent->right - not changed

    if (right_son != nullptr) {
      right_son->parent = grand_parent;
    }

    UpdateSum(grand_parent);
    UpdateSum(parent);
    UpdateSum(node);

    return node;
  }
};

int main()
{
  std::string word;
  std::cin >> word;
  Rope rope(word);
  
  int queries_count;
  std::cin >> queries_count;
  
  for (int counter = 0; counter < queries_count; ++counter)
  {
    int i, j, k;
    std::cin >> i >> j >> k;
    rope.CutAndPaste(i + 1, j + 1, k);
  }
  
  for (int i = 1; i <= static_cast<int>(word.length()); ++i)
  {
    Node* node = rope.Find(i);
    std::cout << node->letter;
  }

  return 0;
}
