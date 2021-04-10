#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;
using std::swap;
using std::pair;
using std::make_pair;

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
        result.push_back({ parent_idx, left_child_idx });
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
      result.push_back({ parent_idx, swap_index });
      std::swap(heap[parent_idx], heap[swap_index]);

      parent_idx = swap_index;
      left_child_idx = 2 * parent_idx + 1;
      right_child_idx = 2 * parent_idx + 2;
    }
  }
}

class HeapBuilder {
 private:
  vector<int> data_;
  vector< pair<int, int> > swaps_;

  void WriteResponse() const {
    cout << swaps_.size() << "\n";
    for (int i = 0; i < swaps_.size(); ++i) {
      cout << swaps_[i].first << " " << swaps_[i].second << "\n";
    }
  }

  void ReadData() {
    int n;
    cin >> n;
    data_.resize(n);
    for(int i = 0; i < n; ++i)
      cin >> data_[i];
  }

  void GenerateSwaps() {
    swaps_.clear();
    // The following naive implementation just sorts 
    // the given sequence using selection sort algorithm
    // and saves the resulting sequence of swaps.
    // This turns the given array into a heap, 
    // but in the worst case gives a quadratic number of swaps.
    //
    // TODO: replace by a more efficient implementation
    /*for (int i = 0; i < data_.size(); ++i)
      for (int j = i + 1; j < data_.size(); ++j) {
        if (data_[i] > data_[j]) {
          swap(data_[i], data_[j]);
          swaps_.push_back(make_pair(i, j));
        }
      }*/

    int size = static_cast<int>(data_.size());
    for (int i = size / 2; i >= 0; --i) {
      SiftDown(data_, static_cast<std::size_t>(i), swaps_);
    }
  }

 public:
  void Solve() {
    ReadData();
    GenerateSwaps();
    WriteResponse();
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  HeapBuilder heap_builder;
  heap_builder.Solve();
  return 0;
}
