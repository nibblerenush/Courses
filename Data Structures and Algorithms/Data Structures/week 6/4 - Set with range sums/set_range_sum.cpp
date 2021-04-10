/*#include <cstdio>

// Splay tree implementation

// Vertex of a splay tree
struct Vertex {
  int key;
  // Sum of all the keys in the subtree - remember to update
  // it after each operation that changes the tree.
  long long sum;
  Vertex* left;
  Vertex* right;
  Vertex* parent;

  Vertex(int key, long long sum, Vertex* left, Vertex* right, Vertex* parent) 
  : key(key), sum(sum), left(left), right(right), parent(parent) {}
};

void update(Vertex* v) {
  if (v == NULL) return;
  v->sum = v->key + (v->left != NULL ? v->left->sum : 0ll) + (v->right != NULL ? v->right->sum : 0ll);
  if (v->left != NULL) {
    v->left->parent = v;
  }
  if (v->right != NULL) {
    v->right->parent = v;
  }
}

void small_rotation(Vertex* v) {
  Vertex* parent = v->parent;
  if (parent == NULL) {
    return;
  }
  Vertex* grandparent = v->parent->parent;
  if (parent->left == v) {
    Vertex* m = v->right;
    v->right = parent;
    parent->left = m;
  } else {
    Vertex* m = v->left;
    v->left = parent;
    parent->right = m;
  }
  update(parent);
  update(v);
  v->parent = grandparent;
  if (grandparent != NULL) {
    if (grandparent->left == parent) {
      grandparent->left = v;
    } else {
      grandparent->right = v;
    }
  }
}

void big_rotation(Vertex* v) {
  if (v->parent->left == v && v->parent->parent->left == v->parent) {
    // Zig-zig
    small_rotation(v->parent);
    small_rotation(v);
  } else if (v->parent->right == v && v->parent->parent->right == v->parent) {
    // Zig-zig
    small_rotation(v->parent);
    small_rotation(v);
  } else {
    // Zig-zag
    small_rotation(v);
    small_rotation(v);
  }  
}

// Makes splay of the given vertex and makes
// it the new root.
void splay(Vertex*& root, Vertex* v) {
  if (v == NULL) return;
  while (v->parent != NULL) {
    if (v->parent->parent == NULL) {
      small_rotation(v);
      break;
    }
    big_rotation(v);
  }
  root = v;
}

// Searches for the given key in the tree with the given root
// and calls splay for the deepest visited node after that.
// If found, returns a pointer to the node with the given key.
// Otherwise, returns a pointer to the node with the smallest
// bigger key (next value in the order).
// If the key is bigger than all keys in the tree, 
// returns NULL.
Vertex* find(Vertex*& root, int key) {
  Vertex* v = root;
  Vertex* last = root;
  Vertex* next = NULL;
  while (v != NULL) {
    if (v->key >= key && (next == NULL || v->key < next->key)) {
      next = v;
    }
    last = v;
    if (v->key == key) {
      break;      
    }
    if (v->key < key) {
      v = v->right;
    } else {
      v = v->left;
    }
  }
  splay(root, last);
  return next;
}

void split(Vertex* root, int key, Vertex*& left, Vertex*& right) {
  right = find(root, key);
  splay(root, right);
  if (right == NULL) {
    left = root;
    return;
  }
  left = right->left;
  right->left = NULL;
  if (left != NULL) {
    left->parent = NULL;
  }
  update(left);
  update(right);
}

Vertex* merge(Vertex* left, Vertex* right) {
  if (left == NULL) return right;
  if (right == NULL) return left;
  Vertex* min_right = right;
  while (min_right->left != NULL) {
    min_right = min_right->left;
  }
  splay(right, min_right);
  right->left = left;
  update(right);
  return right;
}

// Code that uses splay tree to solve the problem

Vertex* root = NULL;

void insert(int x) {
  Vertex* left = NULL;
  Vertex* right = NULL;
  Vertex* new_vertex = NULL;  
  split(root, x, left, right);
  if (right == NULL || right->key != x) {
    new_vertex = new Vertex(x, x, NULL, NULL, NULL);
  }
  root = merge(merge(left, new_vertex), right);
}

void erase(int x) {                   
  // Implement erase yourself

}

bool find(int x) {  
  // Implement find yourself

  return false;
}

long long sum(int from, int to) {
  Vertex* left = NULL;
  Vertex* middle = NULL;
  Vertex* right = NULL;
  split(root, from, left, middle);
  split(middle, to + 1, middle, right);
  long long ans = 0;
  // Complete the implementation of sum
  
  return ans;  
}

const int MODULO = 1000000001;

int main(){
  int n;
  scanf("%d", &n);
  int last_sum_result = 0;
  for (int i = 0; i < n; i++) {
    char buffer[10];
    scanf("%s", buffer);
    char type = buffer[0];
    switch (type) {
      case '+' : {
        int x;
        scanf("%d", &x);
        insert((x + last_sum_result) % MODULO);
      } break;
      case '-' : {
        int x;
        scanf("%d", &x);
        erase((x + last_sum_result) % MODULO);
      } break;            
      case '?' : {
        int x;
        scanf("%d", &x);
        printf(find((x + last_sum_result) % MODULO) ? "Found\n" : "Not found\n");
      } break;
      case 's' : {
        int l, r;
        scanf("%d %d", &l, &r);
        long long res = sum((l + last_sum_result) % MODULO, (r + last_sum_result) % MODULO);
        printf("%lld\n", res);
        last_sum_result = int(res % MODULO);
      }
    }
  }
  return 0;
}*/

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>

struct Node
{
  Node* parent = nullptr;
  Node* left = nullptr;
  Node* right = nullptr;
  std::int64_t value = 0;
  std::int64_t sum = 0;

  ~Node() {
    delete left;
    delete right;
  }
};

void InOrder(Node* root)
{
  if (root == nullptr) {
    return;
  }

  InOrder(root->left);
  std::cout << "value = " << root->value;
  if (root->parent) {
    std::cout << " parent = " << root->parent->value;
  }
  else {
    std::cout << " parent = null";
  }
  std::cout << std::endl;
  InOrder(root->right);
}

class SplayTree
{
public:
  void Insert(std::int64_t value)
  {
    if (m_root == nullptr)
    {
      m_root = new Node;
      m_root->value = value;
      m_root->sum = value;
      return;
    }

    Node* prev_node = Find(m_root, value);
    if (prev_node->value == value)
    {
      m_root = splay(prev_node);
      return;
    }

    Node* new_node = new Node;
    new_node->value = value;
    new_node->sum = value;

    new_node->parent = prev_node;
    prev_node->sum += value;

    if (value > prev_node->value) {
      prev_node->right = new_node;
    }
    else {
      prev_node->left = new_node;
    }
    m_root = splay(new_node);
  }

  void Remove(std::int64_t value)
  {
    if (m_root == nullptr) {
      return;
    }

    Node* remove_node = Find(m_root, value);
    if (remove_node->value != value) {
      m_root = splay(remove_node);
      return;
    }

    remove_node = splay(remove_node);

    Node* remove_left = remove_node->left;
    if (remove_left) { remove_left->parent = nullptr; }

    Node* remove_right = remove_node->right;
    if (remove_right) { remove_right->parent = nullptr; }

    remove_node->left = nullptr;
    remove_node->right = nullptr;
    delete remove_node;

    // Max in left tree
    Node* cur_node = remove_left;
    Node* max_node = nullptr;
    while (cur_node != nullptr)
    {
      max_node = cur_node;
      cur_node = cur_node->right;
    }

    if (max_node)
    {
      m_root = splay(max_node);
      m_root->right = remove_right;
      if (remove_right) { remove_right->parent = m_root; }
    }
    else {
      m_root = remove_right;
    }
  }

  bool Check(std::int64_t value)
  {
    if (m_root == nullptr) {
      return false;
    }

    Node* node = Find(m_root, value);
    m_root = splay(node);
    return m_root->value == value;
  }

  std::int64_t IntervalSum(std::int64_t left, std::int64_t right)
  {
    std::int64_t result = 0;

    if (m_root == nullptr) {
      return result;
    }

    Node* new_root = Find(m_root, left);
    m_root = splay(new_root);

    if (new_root->value >= left && new_root->value <= right) {
      result += new_root->value;
    }

    Node* root_right = new_root->right;
    if (root_right) { root_right->parent = nullptr; }

    Node* sum_node = Find(root_right, right);
    sum_node = splay(sum_node);

    if (sum_node)
    {
      if (sum_node->value >= left && sum_node->value <= right) {
        result += sum_node->value;
      }

      if (sum_node->left) { result += sum_node->left->sum; }
    }

    if (sum_node) { sum_node->parent = new_root; }
    new_root->right = sum_node;

    return result;
  }

  void Print() {
    InOrder(m_root);
  }

  ~SplayTree() {
    delete m_root;
  }

private:
  Node* m_root = nullptr;

  Node* Find(Node* root, std::int64_t value)
  {
    Node* cur_node = root;
    Node* prev_node = nullptr;

    while (cur_node != nullptr)
    {
      prev_node = cur_node;
      if (value == cur_node->value) {
        break;
      }
      else if (value > cur_node->value) {
        cur_node = cur_node->right;
      }
      else {
        cur_node = cur_node->left;
      }
    }

    return prev_node;
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
    node->sum = node->value;
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

std::int64_t f(std::int64_t i, std::int64_t s) {
  return (i + s) % 1'000'000'001;
}

int main()
{
  int queries_count;
  std::cin >> queries_count;

  std::int64_t s = 0;
  SplayTree splay_tree;
  std::ostringstream output;

  for (int counter = 0; counter < queries_count; ++counter)
  {
    char oper;
    std::cin >> oper;

    if (oper == '+')
    {
      std::int64_t i;
      std::cin >> i;
      splay_tree.Insert(f(i, s));
    }
    else if (oper == '-')
    {
      std::int64_t i;
      std::cin >> i;
      splay_tree.Remove(f(i, s));
    }
    else if (oper == '?')
    {
      std::int64_t i;
      std::cin >> i;
      if (splay_tree.Check(f(i, s))) {
        output << "Found\n";
      }
      else {
        output << "Not found\n";
      }
    }
    else if (oper == 's')
    {
      std::int64_t l, r;
      std::cin >> l >> r;
      s = splay_tree.IntervalSum(f(l, s), f(r, s));
      output << s << std::endl;
    }
  }
  std::cout << output.str();
  return 0;
}
