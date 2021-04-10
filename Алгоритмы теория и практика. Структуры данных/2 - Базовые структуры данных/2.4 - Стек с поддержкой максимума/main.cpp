#include <iostream>
#include <stack>
#include <string>

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

int main()
{
  int commands_number;
  std::cin >> commands_number;

  StackWithMax swm;
  for (int i = 0; i < commands_number; ++i)
  {
    std::string command;
    std::cin >> command;

    if (command == "push")
    {
      int v;
      std::cin >> v;
      swm.Push(v);
    }
    else if (command == "pop")
    {
      swm.Pop();
    }
    else if (command == "max")
    {
      std::cout << swm.Max() << '\n';
    }
  }

  return 0;
}
