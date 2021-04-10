#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

struct Node {
  std::unordered_set<Node*> children_set;
  std::size_t position = 0;
  std::size_t size = 0;
  
  ~Node() {
    for (Node* child : children_set) {
      delete child;
    }
  }
};

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
  
  // create root
  std::unique_ptr<Node> root = std::make_unique<Node>();

  // main loop
  for (std::size_t i = 0; i < text.size(); ++i) {
    Node* current_node = root.get();
    std::size_t text_pos = i;
    
    while (true) {
      // find prefix
      Node* next_child = nullptr;
      for (Node* child : current_node->children_set) {
        if (text[text_pos] == text[child->position]) {
          next_child = child;
          break;
        }
      }
      
      // prefix found
      if (next_child) {
        std::size_t child_pos = next_child->position;
        while (child_pos < next_child->position + next_child->size) {
          if (text[child_pos] == text[text_pos]) {
            child_pos++;
            text_pos++;
          }
          else {
            break;
          }
        }
        
        // split
        if (child_pos < next_child->position + next_child->size) {
          Node* split_edge_begin = new Node{};
          Node* split_edge_end = new Node{};

          split_edge_begin->position = next_child->position;
          split_edge_begin->size = child_pos - next_child->position;

          current_node->children_set.insert(split_edge_begin);
          current_node->children_set.erase(next_child);

          split_edge_begin->children_set.insert(next_child);
          split_edge_begin->children_set.insert(split_edge_end);

          next_child->position = child_pos;
          next_child->size = next_child->size - split_edge_begin->size;

          split_edge_end->position = text_pos;
          split_edge_end->size = text.size() - text_pos;
          break;
        }
        // continue traverse tree
        else {
          current_node = next_child;
        }
      }
      // prefix not found
      else {
        Node* new_node = new Node{};
        current_node->children_set.insert(new_node);
        new_node->position = text_pos;
        new_node->size = text.size() - text_pos;
        break;
      }
    }
  }
  
  GetResult(root.get(), text, result);
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
