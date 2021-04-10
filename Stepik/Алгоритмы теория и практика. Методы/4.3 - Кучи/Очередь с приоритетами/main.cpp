#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class PriorityQueue
{
public:
  void Insert(int number)
  {
    m_vector.push_back(number);
    SiftUp();
  }

  int ExtractMax()
  {
    int result = m_vector[0];
    SiftDown();
    return result;
  }

private:
  void SiftUp()
  {
    int child_index = static_cast<int>(m_vector.size()) - 1;
    int parent_index = (child_index - 1) / 2;
    
    while (parent_index >= 0 && m_vector[child_index] > m_vector[parent_index])
    {
      std::swap(m_vector[child_index], m_vector[parent_index]);
      child_index = parent_index;
      parent_index = (child_index - 1) / 2;
    }
  }

  void SiftDown()
  {
    m_vector[0] = m_vector.back();
    m_vector.pop_back();
    
    std::size_t parent_idx = 0;
    std::size_t left_child_idx = 2 * parent_idx + 1;
    std::size_t right_child_idx = 2 * parent_idx + 2;
    
    while (left_child_idx < m_vector.size())
    {
      if (right_child_idx >= m_vector.size())
      {
        if (m_vector[parent_idx] < m_vector[left_child_idx])
        {
          std::swap(m_vector[parent_idx], m_vector[left_child_idx]);
        }
        break;
      }
      else
      {
        std::size_t swap_index;
        if (m_vector[parent_idx] < m_vector[left_child_idx] && m_vector[parent_idx] < m_vector[right_child_idx])
        {
          swap_index = m_vector[left_child_idx] > m_vector[right_child_idx] ? left_child_idx : right_child_idx;
        }
        else if (m_vector[parent_idx] < m_vector[left_child_idx])
        {
          swap_index = left_child_idx;
        }
        else if (m_vector[parent_idx] < m_vector[right_child_idx])
        {
          swap_index = right_child_idx;
        }
        else
        {
          break;
        }
        std::swap(m_vector[parent_idx], m_vector[swap_index]);

        parent_idx = swap_index;
        left_child_idx = 2 * parent_idx + 1;
        right_child_idx = 2 * parent_idx + 2;
      }
    }
  }

private:
  std::vector<int> m_vector;
};

int main()
{  
  PriorityQueue priority_queue;
  
  int number_of_operation;
  std::cin >> number_of_operation;
  
  for (int i = 0; i < number_of_operation; ++i)
  {
    std::string operation;
    std::cin >> operation;

    if (operation == "Insert")
    {
      int number;
      std::cin >> number;
      priority_queue.Insert(number);
    }
    else if (operation == "ExtractMax")
    {
      std::cout << priority_queue.ExtractMax() << std::endl;
    }
  }
  
  return 0;
}
