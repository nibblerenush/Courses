#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
void RemoveDuplicates(std::vector<T>& elements)
{
  std::sort(elements.begin(), elements.end());
  typename std::vector<T>::iterator it = std::unique(elements.begin(), elements.end());
  elements.erase(it, elements.end());
}

void TestRemoveDuplicates()
{
  {
    std::vector<int> v = { 1, 2, 4, 2, 2, 1, 10, 90, 10 };
    std::vector<int> expected = { 1, 2, 4, 10, 90 };
    RemoveDuplicates(v);
    AssertEqual(v, expected, "");
  }

  {
    std::vector<int> v = { 2, 1 };
    std::vector<int> expected = { 1, 2 };
    RemoveDuplicates(v);
    AssertEqual(v, expected, "");
  }

  {
    std::vector<int> v = { 1 };
    std::vector<int> expected = { 1 };
    RemoveDuplicates(v);
    AssertEqual(v, expected, "");
  }

  {
    std::vector<int> v = {};
    std::vector<int> expected = {};
    RemoveDuplicates(v);
    AssertEqual(v, expected, "");
  }
}

void TestAll()
{
  TestRunner tr;
  tr.RunTest(TestRemoveDuplicates, "TestRemoveDuplicates");
}

int main()
{
  TestAll();
  return 0;
}
