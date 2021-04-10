#include "test_runner.h"

#include <algorithm>
#include <cstdint>
#include <future>
#include <numeric>
#include <vector>
using namespace std;

template <typename Iterator>
class Page
{
public:
  Page(Iterator page_begin, Iterator page_end);
  Iterator begin() const;
  Iterator end() const;
  std::size_t size() const;
private:
  Iterator m_page_begin;
  Iterator m_page_end;
};

template <typename Iterator>
Page<Iterator>::Page(Iterator page_begin, Iterator page_end) : m_page_begin(page_begin), m_page_end(page_end)
{}

template <typename Iterator>
Iterator Page<Iterator>::begin() const
{
  return m_page_begin;
}

template <typename Iterator>
Iterator Page<Iterator>::end() const
{
  return m_page_end;
}

template <typename Iterator>
std::size_t Page<Iterator>::size() const
{
  std::size_t result = 0;
  Iterator start = m_page_begin;
  while (start != m_page_end)
  {
    start++;
    result++;
  }
  return result;
}

template <typename Iterator>
class Paginator
{
public:
  Paginator(Iterator source_begin, Iterator source_end, std::size_t page_size);
  typename std::vector<Page<Iterator>>::const_iterator begin() const;
  typename std::vector<Page<Iterator>>::const_iterator end() const;
  std::size_t size() const;
private:
  std::vector<Page<Iterator>> m_pages;
};

template <typename Iterator>
Paginator<Iterator>::Paginator(Iterator source_begin, Iterator source_end, std::size_t page_size)
{
  std::size_t counter = 0;
  Iterator start = source_begin;
  while (source_begin != source_end)
  {
    if (counter == page_size)
    {
      counter = 0;
      m_pages.push_back({ start, source_begin });
      start = source_begin;
    }

    counter++;
    source_begin++;
  }

  if (start != source_end)
  {
    m_pages.push_back({ start, source_end });
  }
}

template <typename Iterator>
typename std::vector<Page<Iterator>>::const_iterator Paginator<Iterator>::begin() const
{
  return m_pages.begin();
}

template <typename Iterator>
typename std::vector<Page<Iterator>>::const_iterator Paginator<Iterator>::end() const
{
  return m_pages.end();
}

template <typename Iterator>
std::size_t Paginator<Iterator>::size() const
{
  return m_pages.size();
}

template <typename C>
auto Paginate(C& c, size_t page_size)
{
  return Paginator(c.begin(), c.end(), page_size);
}

template <typename C>
std::int64_t SumPart(C container)
{
  std::int64_t result = 0;
  for (const auto& row : container)
  {
    result += std::accumulate(row.begin(), row.end(), 0);
  }
  return result;
}

std::int64_t CalculateMatrixSum(const std::vector<std::vector<int>>& matrix)
{
  std::vector<std::future<std::int64_t>> futures;
  for (const auto& page : Paginate(matrix, 2000))
  {
    futures.push_back(std::async([page] ()
    {
      return SumPart(page);
    }));
  }

  std::int64_t result = 0;
  for (auto& fut : futures)
  {
    result += fut.get();
  }

  return result;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
