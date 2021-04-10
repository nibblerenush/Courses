#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

void SiftDown(std::vector<int>& heap, std::size_t index, std::vector<std::pair<int, int>>& result)
{
  std::size_t parent_idx = index;
  std::size_t left_child_idx = 2 * parent_idx + 1;
  std::size_t right_child_idx = 2 * parent_idx + 2;
  
  while (left_child_idx < heap.size())
  {
    if (right_child_idx >= heap.size())
    {
      if (heap[parent_idx] > heap[left_child_idx])
      {
        result.push_back({parent_idx, left_child_idx});
        std::swap(heap[parent_idx], heap[left_child_idx]);
      }
      break;
    }
    else
    {
      std::size_t swap_index;
      if (heap[parent_idx] > heap[left_child_idx] && heap[parent_idx] > heap[right_child_idx]) {
        swap_index = heap[left_child_idx] < heap[right_child_idx] ? left_child_idx : right_child_idx;
      }
      else if (heap[parent_idx] > heap[left_child_idx]) {
        swap_index = left_child_idx;
      }
      else if (heap[parent_idx] > heap[right_child_idx]) {
        swap_index = right_child_idx;
      }
      else {
        break;
      }
      result.push_back({parent_idx, swap_index});
      std::swap(heap[parent_idx], heap[swap_index]);
      
      parent_idx = swap_index;
      left_child_idx = 2 * parent_idx + 1;
      right_child_idx = 2 * parent_idx + 2;
    }
  }
}

std::vector<std::pair<int, int>> BuildHeap(std::vector<int>& numbers)
{
  std::vector<std::pair<int, int>> result;
  int size = static_cast<int>(numbers.size());
  for (int i = size / 2; i >= 0; --i) {
    SiftDown(numbers, static_cast<std::size_t>(i), result);
  }
  return result;
}

int main()
{
  int numbers_count;
  std::cin >> numbers_count;

  std::vector<int> numbers(numbers_count);
  for (int i = 0; i < numbers_count; ++i) {
    std::cin >> numbers[i];
  }
  
  auto result = BuildHeap(numbers);
  std::cout << result.size() << '\n';
  for (const auto& pair_index : result) {
    std::cout << pair_index.first << ' ' << pair_index.second << '\n';
  }

  return 0;
}
