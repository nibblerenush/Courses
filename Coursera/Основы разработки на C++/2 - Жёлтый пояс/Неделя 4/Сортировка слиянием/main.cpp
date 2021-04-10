#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <vector>

/*template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
  if (range_end - range_begin < 2)
  {
    return;
  }
  
  std::vector<typename RandomIt::value_type> elements(range_begin, range_end);
  RandomIt range_middle = elements.begin() + elements.size() / 2;
  
  MergeSort(elements.begin(), range_middle);
  MergeSort(range_middle, elements.end());
  
  std::merge(elements.begin(), range_middle, range_middle, elements.end(), range_begin);
}*/

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
  if (range_end - range_begin < 2)
  {
    return;
  }

  std::vector<typename RandomIt::value_type> elements(range_begin, range_end);
  RandomIt one_part = elements.begin() + elements.size() / 3;
  RandomIt two_part = elements.begin() + 2 * elements.size() / 3;
  
  MergeSort(elements.begin(), one_part);
  MergeSort(one_part, two_part);
  MergeSort(two_part, elements.end());

  std::vector<typename RandomIt::value_type> temp;
  std::merge(elements.begin(), one_part, one_part, two_part, std::back_inserter(temp));
  std::merge(temp.begin(), temp.end(), two_part, elements.end(), range_begin);
}

void TestMergeSort()
{
  {
    std::vector<int> v = { 6, 4, 7, 6, 4, 4, 0, 1 };
    std::vector<int> expected = { 0, 1, 4, 4, 4, 6, 6, 7 };
    MergeSort(v.begin(), v.end());
    AssertEqual(v, expected, "");
  }

  {
    std::vector<int> v = { 4, 2, 3, 5, 1 };
    std::vector<int> expected = { 1, 2, 3, 4, 5 };
    MergeSort(v.begin(), v.end());
    AssertEqual(v, expected, "");
  }

  {
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };
    MergeSort(v.begin(), v.end());
    AssertEqual(v, expected, "");
  }

  {
    std::vector<int> v = { 2 };
    std::vector<int> expected = { 2 };
    MergeSort(v.begin(), v.end());
    AssertEqual(v, expected, "");
  }

  {
    std::vector<int> v = {};
    std::vector<int> expected = {};
    MergeSort(v.begin(), v.end());
    AssertEqual(v, expected, "");
  }
}

void TestAll()
{
  TestRunner tr;
  tr.RunTest(TestMergeSort, "TestMergeSort");
}

int main()
{
  TestAll();

  std::vector<int> v = { 6, 4, 7, 6, 4, 4, 0, 1};
  MergeSort(std::begin(v), std::end(v));
  for (int x : v) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
  return 0;
}
