#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

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

std::string GetNonSharedSubstring(const string& text) {
  std::size_t str_pos = 0;
  std::size_t min_size = std::numeric_limits<std::size_t>::max();
  bool sharp = false;

  // create root
  std::unique_ptr<Node> root = std::make_unique<Node>();

  // main loop
  for (std::size_t i = 0; i < text.size(); ++i) {
    std::size_t str_size = 0;
    if (i != 0 && text[i - 1] == '#') {
      sharp = true;
    }
    
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

          if (sharp && text[text_pos] != '$') {
            str_size += split_edge_begin->size + 1;
            if (str_size < min_size) {
              min_size = str_size;
              str_pos = i;
            }
          }

          break;
        }
        // continue traverse tree
        else {
          current_node = next_child;
          if (sharp) {
            str_size += current_node->size;
          }
        }
      }
      // prefix not found
      else {
        Node* new_node = new Node{};
        current_node->children_set.insert(new_node);
        new_node->position = text_pos;
        new_node->size = text.size() - text_pos;

        if (sharp && text[text_pos] != '$') {
          str_size += 1;
          if (str_size < min_size) {
            min_size = str_size;
            str_pos = i;
          }
        }

        break;
      }
    }
  }

  return text.substr(str_pos, min_size);
}

string solve(string p, string q) {
  std::ostringstream oss;
  oss << q << '#' << p << '$';
  return GetNonSharedSubstring(oss.str());
}

int main (void)
{
	string p;
	cin >> p;
	string q;
	cin >> q;

	string ans = solve (p, q);

	cout << ans << endl;

	return 0;
}
