#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

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

  int Max() {
    return m_max_stack.top();
  }

  int Top() {
    return m_stack.top();
  }

  bool Empty() {
    return m_stack.empty();
  }

private:
  std::stack<int> m_stack;
  std::stack<int> m_max_stack;
};

class QueueWithMax
{
public:
  void Push(int v) {
    m_push_stack.Push(v);
  }

  void Pop()
  {
    if (m_pop_stack.Empty())
    {
      while (!m_push_stack.Empty())
      {
        m_pop_stack.Push(m_push_stack.Top());
        m_push_stack.Pop();
      }
    }
    m_pop_stack.Pop();
  }
  
  int Max()
  {
    if (!m_push_stack.Empty() && !m_pop_stack.Empty()) {
      return std::max(m_push_stack.Max(), m_pop_stack.Max());
    }
    else {
      return !m_push_stack.Empty() ? m_push_stack.Max() : m_pop_stack.Max();
    }
  }

private:
  StackWithMax m_push_stack;
  StackWithMax m_pop_stack;
};

int main()
{
  int numbers_count;
  std::cin >> numbers_count;

  std::vector<int> numbers;
  numbers.reserve(numbers_count);
  for (int i = 0; i < numbers_count; ++i)
  {
    int number;
    std::cin >> number;
    numbers.push_back(number);
  }

  int window_size;
  std::cin >> window_size;

  QueueWithMax qwm;
  for (int i = 0; i < window_size - 1; ++i) {
    qwm.Push(numbers[i]);
  }

  bool first = true;
  for (int i = window_size - 1; i < numbers_count; ++i)
  {
    if (first) {
      first = false;
    }
    else {
      qwm.Pop();
    }
    qwm.Push(numbers[i]);
    std::cout << qwm.Max() << ' ';
  }
  std::cout << std::endl;

  return 0;
}
