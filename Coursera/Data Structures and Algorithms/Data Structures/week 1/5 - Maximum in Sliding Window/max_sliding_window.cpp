#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::max;

void max_sliding_window_naive(vector<int> const & A, int w) {
    for (size_t i = 0; i < A.size() - w + 1; ++i) {
        int window_max = A.at(i);
        for (size_t j = i + 1; j < i + w; ++j)
            window_max = max(window_max, A.at(j));

        cout << window_max << " ";
    }

    return;
}

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


void max_sliding_window_fast(vector<int> const& A, int w) {
  QueueWithMax qwm;
  for (int i = 0; i < w - 1; ++i) {
    qwm.Push(A[i]);
  }

  bool first = true;
  for (int i = w - 1; i < (int)A.size(); ++i)
  {
    if (first) {
      first = false;
    }
    else {
      qwm.Pop();
    }
    qwm.Push(A[i]);
    std::cout << qwm.Max() << ' ';
  }
  std::cout << std::endl;
}

int main() {
    int n = 0;
    cin >> n;

    vector<int> A(n);
    for (size_t i = 0; i < n; ++i)
        cin >> A.at(i);

    int w = 0;
    cin >> w;

    //max_sliding_window_naive(A, w);
    max_sliding_window_fast(A, w);

    return 0;
}
