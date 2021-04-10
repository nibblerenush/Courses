#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

struct Node {
  //Node* parent = nullptr;
  //std::unordered_map<char, Node*> children_map;
  std::unordered_set<Node*> children_set;
  std::size_t position = 0;
  std::size_t size = 0;
  
  ~Node() {
    for (Node* child : children_set) {
      delete child;
    }
  }
};

/*void BuildSuffixTree(Node* root) {
  if (!root->used) {
    return;
  }
  
  // node with one child
  if (root->parent != nullptr && root->children.size() == 1) {
    Node* parent = root->parent;
    Node* child = *(root->children.begin());
    
    root->parent = nullptr;
    root->children.clear();
    
    child->parent = parent;
    // Set root unused
    root->used = false;
    parent->children.push_back(child);

    child->position = root->position;
    child->size += root->size;

    BuildSuffixTree(child);
  }
  else {
    for (std::size_t i = 0; i < root->children.size(); ++i) {
      BuildSuffixTree(root->children[i]);
    }
  }
}*/

void GetResult(Node* root, const std::string& text, std::vector<std::string>& result) {
  for (Node* node : root->children_set) {
    result.push_back(text.substr(node->position, node->size));
    GetResult(node, text, result);
  }
}

// Build a suffix tree of the string text and return a vector
// with all of the labels of its edges (the corresponding 
// substrings of the text) in any order.
vector<string> ComputeSuffixTreeEdges(const string& text) {
  vector<string> result;
  // Implement this function yourself
  
  // Build Suffix Trie
  /*Node* root = new Node();
  for (std::size_t i = 0; i < text.size(); ++i) {
    Node* current_node = root;
    for (std::size_t j = i; j < text.size(); ++j) {
      char current_letter = text[j];
      
      // find next node
      // auto iter = std::find_if(current_node->children.begin(), current_node->children.end(), [c](const Node* node) { return node->c == c; });

      if (current_node->children_set.empty()) {
        Node* new_node = new Node{};
        new_node->position = j;
        new_node->size = text.size() - j;
        current_node->children_set.insert(new_node);
        break;
      }

      std::queue<Node*> queue;
      queue.push(current_node);

      while (!queue.empty()) {

      }

      
      if (current_node->children_map.count(current_letter)) {
        current_node = current_node->children_map.at(current_letter);
      }
      else {
        Node* new_node = new Node();
        new_node->parent = current_node;
        new_node->position = j;
        new_node->size = 1;

        current_node->children_map.emplace(current_letter, new_node);
        //current_node->children_set.insert(new_node);
        current_node = new_node;
      }
    }
  }*/
  
  Node* root = new Node{};
  for (std::size_t i = 0; i < text.size(); ++i) {
    std::queue<Node*> queue;
    queue.push(root);
    
    std::size_t text_pos = i;
    while (!queue.empty()) {
      Node* current_node = queue.front();
      queue.pop();

      // 0 step
      if (text[text_pos] == '$') {
        Node* new_node = new Node{};
        new_node->position = text_pos;
        new_node->size = 1;
        current_node->children_set.insert(new_node);
        break;
      }
      
      // 1 step
      if (current_node == root) {
        bool prefix_found = false;
        for (Node* child : current_node->children_set) {
          if (text[i] == text[child->position]) {
            prefix_found = true;
            break;
          }
        }

        if (!prefix_found) {
          Node* new_node = new Node{};
          new_node->position = i;
          new_node->size = text.size() - i;
          current_node->children_set.insert(new_node);

          /*result.clear();
          GetResult(root, text, result);
          for (int i = 0; i < result.size(); ++i) {
            cout << result[i] << endl;
          }
          cout << "===" << endl;*/

          break;
        }
      }

      // 2 step
      std::size_t child_counter = 0;
      for (Node* child : current_node->children_set) {
        std::size_t child_pos = child->position;
        bool right_child = false;

        while (child_pos < child->position + child->size) {
          if (text[child_pos] == text[text_pos]) {
            child_pos++;
            text_pos++;
            right_child = true;
          }
          else {
            break;
          }
        }

        if (right_child) {
          // split
          if (child_pos < child->position + child->size) {
            Node* split_edge_begin = new Node{};
            Node* split_edge_end = new Node{};

            split_edge_begin->position = child->position;
            split_edge_begin->size = child_pos - child->position;

            current_node->children_set.insert(split_edge_begin);
            current_node->children_set.erase(child);

            split_edge_begin->children_set.insert(child);
            split_edge_begin->children_set.insert(split_edge_end);

            child->position = child_pos;
            child->size = child->size - split_edge_begin->size;

            split_edge_end->position = text_pos;
            split_edge_end->size = text.size() - text_pos;
          }
          // another way
          else {
            queue.push(child);
          }

          /*result.clear();
          GetResult(root, text, result);
          for (int i = 0; i < result.size(); ++i) {
            cout << result[i] << endl;
          }
          cout << "===" << endl;*/

          break;
        }
        child_counter++;
      }

      if (child_counter == current_node->children_set.size()) {
        Node* new_node = new Node{};
        current_node->children_set.insert(new_node);
        new_node->position = text_pos;
        new_node->size = text.size() - text_pos;
        break;
      }
    }
  }

  // Build Suffix Tree
  /*std::queue<Node*> queue;
  queue.push(root);

  while (!queue.empty()) {
    Node* current_node = queue.front();
    queue.pop();
    
    if (current_node->parent != nullptr && current_node->children_set.size() == 1) {
      Node* parent = current_node->parent;
      Node* child = *current_node->children_set.begin();
      
      current_node->parent = nullptr;
      current_node->children_set.clear();

      child->parent = parent;
      parent->children_set.erase(current_node);
      parent->children_set.insert(child);

      child->position = current_node->position;
      child->size += current_node->size;

      delete current_node;
      queue.push(child);
    }
    else {
      for (Node* node : current_node->children_set) {
        queue.push(node);
      }
    }
  }*/
  
  //BuildSuffixTree(root);
  GetResult(root, text, result);
  delete root;
  
  return result;
}

int main() {
  string text;
  cin >> text;
  vector<string> edges = ComputeSuffixTreeEdges(text);
  for (int i = 0; i < edges.size(); ++i) {
    cout << edges[i] << endl;
  }
  return 0;
}
