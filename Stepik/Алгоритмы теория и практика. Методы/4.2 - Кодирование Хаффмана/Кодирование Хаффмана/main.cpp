#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>

struct Node
{
  const Node* left = nullptr;
  const Node* right = nullptr;
  char symbol = 0;
  int frequency = 0;

  ~Node()
  {
    delete left;
    delete right;
  }
};

class Compare
{
public:
  bool operator()(const Node* lhs, const Node* rhs) const
  {
    return lhs->frequency > rhs->frequency;
  }
};

void CreateCode(const Node* root, std::string code, std::map<char, std::string>& symbol_code)
{
  if (root == nullptr)
  {
    return;
  }

  if (root->left == nullptr && root->right == nullptr)
  {
    if (!code.empty())
    {
      symbol_code[root->symbol] = code;
    }
    else
    {
      symbol_code[root->symbol] = "0";
    }
    return;
  }

  CreateCode(root->left, code + '0', symbol_code);
  CreateCode(root->right, code + '1', symbol_code);
}

int main()
{
  std::string input_str;
  std::getline(std::cin, input_str);
  
  std::map<char, int> sym_freq;
  for (const auto& sym : input_str)
  {
    sym_freq[sym]++;
  }
  
  std::priority_queue<Node*, std::vector<Node*>, Compare> queue;
  for (const auto& item : sym_freq)
  {
    queue.push(new Node{ nullptr, nullptr, item.first, item.second });
  }

  while (queue.size() > 1)
  {
    const Node* left_node = queue.top();
    queue.pop();
    const Node* right_node = queue.top();
    queue.pop();
    queue.push(new Node{ left_node, right_node, 0, left_node->frequency + right_node->frequency });
  }
  const Node* root = queue.top();
  queue.pop();
  
  std::map<char, std::string> symbol_code;
  CreateCode(root, "", symbol_code);
  delete root;

  std::ostringstream encode_str;
  for (const auto& sym : input_str)
  {
    encode_str << symbol_code[sym];
  }

  std::cout << sym_freq.size() << ' ' << encode_str.str().length() << '\n';
  for (const auto& item : symbol_code)
  {
    std::cout << item.first << ": " << item.second << '\n';
  }
  std::cout << encode_str.str();

  return 0;
}
