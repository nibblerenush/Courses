#include "test_runner.h"
#include "profile.h"

#include <future>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <utility>
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

struct Stats
{
  std::map<std::string, int> word_frequences;

  void operator += (const Stats& other);
};

void Stats::operator += (const Stats& other)
{
  for (const auto& [key, value] : other.word_frequences)
  {
    word_frequences[key] += value;
  }
}

Stats ExploreLine(const std::set<std::string>& key_words, const std::string& line)
{
  Stats result;
  std::string_view sv_line(line);
  
  while (true)
  {
    std::size_t space = sv_line.find(' ');
    std::string_view sv_word = sv_line.substr(0, space);
    std::string word(sv_word);
    
    if (key_words.find(word) != key_words.end())
    {
      result.word_frequences[word]++;
    }
    
    if (space == sv_line.npos)
    {
      break;
    }
    
    sv_line.remove_prefix(space + 1);
  }
  
  return result;
}

template <typename C>
Stats ExploreKeyWordsSingleThread(const set<string>& key_words, C lines)
{
  Stats result;
  for (const auto& line : lines)
  {
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWords(const std::set<std::string>& key_words, istream& input)
{
  std::vector<std::future<Stats>> futures;
  std::vector<std::string> lines;
  
  for (string line; getline(input, line); )
  {
    lines.push_back(std::move(line));
  }
  
  for (const auto& page : Paginate(lines, 5000))
  {
    futures.push_back(std::async([&key_words, page] ()
    {
      return ExploreKeyWordsSingleThread(key_words, page);
    }));
  }

  Stats result;
  for (auto& fut : futures)
  {
    result += fut.get();
  }
  return result;
}

void TestBasic() {
  const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
  return 0;
}
