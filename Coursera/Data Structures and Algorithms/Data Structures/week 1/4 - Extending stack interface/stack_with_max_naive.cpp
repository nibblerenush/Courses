#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <stack>

using std::cin;
using std::string;
using std::vector;
using std::cout;
using std::max_element;

/*class StackWithMax {
    vector<int> stack;

  public:

    void Push(int value) {
        stack.push_back(value);
    }

    void Pop() {
        assert(stack.size());
        stack.pop_back();
    }

    int Max() const {
        assert(stack.size());
        return *max_element(stack.begin(), stack.end());
    }
};*/

class StackWithMax
{
public:
  void Push(int v)
  {
    m_stack.push(v);
    if (m_max_stack.empty()) {
      m_max_stack.push(v);
    }
    else
    {
      if (v > m_max_stack.top()) {
        m_max_stack.push(v);
      }
      else {
        m_max_stack.push(m_max_stack.top());
      }
    }
  }

  void Pop()
  {
    m_stack.pop();
    m_max_stack.pop();
  }

  int Max()
  {
    return m_max_stack.top();
  }

private:
  std::stack<int> m_stack;
  std::stack<int> m_max_stack;
};

int main() {
    int num_queries = 0;
    cin >> num_queries;

    string query;
    string value;

    StackWithMax stack;

    for (int i = 0; i < num_queries; ++i) {
        cin >> query;
        if (query == "push") {
            cin >> value;
            stack.Push(std::stoi(value));
        }
        else if (query == "pop") {
            stack.Pop();
        }
        else if (query == "max") {
            cout << stack.Max() << "\n";
        }
        else {
            assert(0);
        }
    }
    return 0;
}
