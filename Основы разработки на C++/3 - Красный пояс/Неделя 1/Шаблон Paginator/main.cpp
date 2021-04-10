#include "test_runner.h"

#include <iostream>
#include <numeric>
#include <string>
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
auto Paginate(C & c, size_t page_size)
{
  return Paginator(c.begin(), c.end(), page_size);
}

void TestPageCounts() {
  vector<int> v(15);

  ASSERT_EQUAL(Paginate(v, 1).size(), v.size());
  ASSERT_EQUAL(Paginate(v, 3).size(), 5u);
  ASSERT_EQUAL(Paginate(v, 5).size(), 3u);
  ASSERT_EQUAL(Paginate(v, 4).size(), 4u);
  ASSERT_EQUAL(Paginate(v, 15).size(), 1u);
  ASSERT_EQUAL(Paginate(v, 150).size(), 1u);
  ASSERT_EQUAL(Paginate(v, 14).size(), 2u);
}

void TestLooping() {
  vector<int> v(15);
  iota(begin(v), end(v), 1);

  Paginator<vector<int>::iterator> paginate_v(v.begin(), v.end(), 6);
  ostringstream os;
  for (const auto& page : paginate_v) {
    for (int x : page) {
      os << x << ' ';
    }
    os << '\n';
  }

  ASSERT_EQUAL(os.str(), "1 2 3 4 5 6 \n7 8 9 10 11 12 \n13 14 15 \n");
}

void TestModification() {
  vector<string> vs = { "one", "two", "three", "four", "five" };
  for (auto page : Paginate(vs, 2)) {
    for (auto& word : page) {
      word[0] = toupper(word[0]);
    }
  }

  const vector<string> expected = { "One", "Two", "Three", "Four", "Five" };
  ASSERT_EQUAL(vs, expected);
}

void TestPageSizes() {
  string letters(26, ' ');

  Paginator letters_pagination(letters.begin(), letters.end(), 11);
  vector<size_t> page_sizes;
  for (const auto& page : letters_pagination) {
    page_sizes.push_back(page.size());
  }

  const vector<size_t> expected = { 11, 11, 4 };
  ASSERT_EQUAL(page_sizes, expected);
}

void TestConstContainer() {
  const string letters = "abcdefghijklmnopqrstuvwxyz";

  vector<string> pages;
  for (const auto& page : Paginate(letters, 10)) {
    pages.push_back(string(page.begin(), page.end()));
  }

  const vector<string> expected = { "abcdefghij", "klmnopqrst", "uvwxyz" };
  ASSERT_EQUAL(pages, expected);
}

void TestPagePagination() {
  vector<int> v(22);
  iota(begin(v), end(v), 1);

  vector<vector<int>> lines;
  for (const auto& split_by_9 : Paginate(v, 9)) {
    for (const auto& split_by_4 : Paginate(split_by_9, 4)) {
      lines.push_back({});
      for (int item : split_by_4) {
        lines.back().push_back(item);
      }
    }
  }

  const vector<vector<int>> expected = {
      {1, 2, 3, 4},
      {5, 6, 7, 8},
      {9},
      {10, 11, 12, 13},
      {14, 15, 16, 17},
      {18},
      {19, 20, 21, 22}
  };
  ASSERT_EQUAL(lines, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestPageCounts);
  RUN_TEST(tr, TestLooping);
  RUN_TEST(tr, TestModification);
  RUN_TEST(tr, TestPageSizes);
  RUN_TEST(tr, TestConstContainer);
  RUN_TEST(tr, TestPagePagination);
}
