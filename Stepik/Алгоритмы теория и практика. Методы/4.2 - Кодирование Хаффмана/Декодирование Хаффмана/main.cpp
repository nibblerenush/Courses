#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

struct Node
{
  Node* left = nullptr;
  Node* right = nullptr;
  char symbol = 0;

  ~Node()
  {
    delete left;
    delete right;
  }
};

int main()
{
  int number_of_symbols, encode_str_length;
  std::cin >> number_of_symbols >> encode_str_length;

  std::unordered_map<char, std::string> symbol_code;
  for (int i = 0; i < number_of_symbols; ++i)
  {
    char symbol;
    std::cin >> symbol;
    std::cin.ignore(2);
    std::string code;
    std::cin >> code;
    symbol_code[symbol] = std::move(code);
  }

  std::string encode_str;
  std::cin >> encode_str;

  Node* root = new Node;
  for (const auto& item : symbol_code)
  {
    const char& symbol = item.first;
    const std::string& code = item.second;
    
    Node* cur_node = root;
    for (const auto& bit : code)
    {
      if (bit == '0')
      {
        if (cur_node->left == nullptr)
        {
          cur_node->left = new Node;
        }
        cur_node = cur_node->left;
      }
      else if (bit == '1')
      {
        if (cur_node->right == nullptr)
        {
          cur_node->right = new Node;
        }
        cur_node = cur_node->right;
      }
    }
    cur_node->symbol = symbol;
  }
  
  std::string result;
  Node* cur_node = root;
  for (const auto& bit : encode_str)
  {
    if (bit == '0')
    {
      cur_node = cur_node->left;
    }
    else if (bit == '1')
    {
      cur_node = cur_node->right;
    }

    if (cur_node->symbol != 0)
    {
      result.push_back(cur_node->symbol);
      cur_node = root;
    }
  }

  std::cout << result;
  delete root;
  return 0;
}
