#include "test_runner.h"

#include <cstdint>
#include <iterator>
#include <list>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, std::uint32_t step_size)
{
  std::list<typename RandomIt::value_type> pool;
  for (RandomIt iter = first; iter != last; iter++)
  {
    pool.push_back(std::move(*iter));
  }
  
  auto cur_iter = pool.begin();
  while (!pool.empty())
  {
    *(first++) = std::move(*cur_iter);
    cur_iter = pool.erase(cur_iter);
    if (pool.empty()) { break; }
    if (cur_iter == pool.end()) { cur_iter = pool.begin(); }
    
    for (std::uint32_t i = 1; i < step_size; ++i)
    {
      if (cur_iter == pool.end())
      {
        cur_iter = pool.begin();
      }
      else
      {
        cur_iter = std::next(cur_iter, 1);
        if (cur_iter == pool.end()) { cur_iter = pool.begin(); }
      }
    }
  } 
}

vector<int> MakeTestVector() {
  vector<int> numbers(10);
  iota(begin(numbers), end(numbers), 0);
  return numbers;
}

void TestIntVector() {
  const vector<int> numbers = MakeTestVector();
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
    ASSERT_EQUAL(numbers_copy, numbers);
  }
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
    ASSERT_EQUAL(numbers_copy, vector<int>({ 0, 3, 6, 9, 4, 8, 5, 2, 7, 1 }));
  }
}

// ��� ����������� ���, ������� ������� ��� ���������, ��� ���� ����������
// ������� MakeJosephusPermutation �� ��������� ����������� ��������.
// ������ ��, ��������, �� ��������� ��� �� �������, ������ �� ���������,
// ������ �� ������� ������ ���, ����� � ����� ��� move-��������� �
// � ����� ������������� �����

struct NoncopyableInt {
  int value;

  NoncopyableInt(const NoncopyableInt&) = delete;
  NoncopyableInt& operator=(const NoncopyableInt&) = delete;

  NoncopyableInt(NoncopyableInt&&) = default;
  NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
  return lhs.value == rhs.value;
}

ostream& operator << (ostream& os, const NoncopyableInt& v) {
  return os << v.value;
}

void TestAvoidsCopying() {
  vector<NoncopyableInt> numbers;
  numbers.push_back({ 1 });
  numbers.push_back({ 2 });
  numbers.push_back({ 3 });
  numbers.push_back({ 4 });
  numbers.push_back({ 5 });

  MakeJosephusPermutation(begin(numbers), end(numbers), 2);

  vector<NoncopyableInt> expected;
  expected.push_back({ 1 });
  expected.push_back({ 3 });
  expected.push_back({ 5 });
  expected.push_back({ 4 });
  expected.push_back({ 2 });

  ASSERT_EQUAL(numbers, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  RUN_TEST(tr, TestAvoidsCopying);
  return 0;
}
