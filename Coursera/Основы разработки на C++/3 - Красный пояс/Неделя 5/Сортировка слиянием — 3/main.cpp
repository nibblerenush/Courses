#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
  if (range_end - range_begin < 2)
  {
    return;
  }
  
  std::vector<typename RandomIt::value_type> elements;
  std::move(range_begin, range_end, std::back_inserter(elements));

  RandomIt one_part = elements.begin() + elements.size() / 3;
  RandomIt two_part = elements.begin() + 2 * elements.size() / 3;

  MergeSort(elements.begin(), one_part);
  MergeSort(one_part, two_part);
  MergeSort(two_part, elements.end());

  std::vector<typename RandomIt::value_type> temp;

  std::merge(std::make_move_iterator(elements.begin()),
    std::make_move_iterator(one_part),
    std::make_move_iterator(one_part),
    std::make_move_iterator(two_part),
    std::back_inserter(temp));

  std::merge(std::make_move_iterator(temp.begin()),
    std::make_move_iterator(temp.end()),
    std::make_move_iterator(two_part),
    std::make_move_iterator(elements.end()),
    range_begin);
}

void TestIntVector() {
  vector<int> numbers = { 6, 1, 3, 9, 1, 9, 8, 12, 1 };
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
