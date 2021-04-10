#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

/*template <typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, char prefix)
{
  std::string prefix_str{ prefix };
  auto first = std::lower_bound(range_begin, range_end, prefix_str);

  prefix_str[0]++;
  auto second = std::lower_bound(range_begin, range_end, prefix_str);

  return std::make_pair(first, second);
}*/

template <typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, const std::string& prefix)
{
  std::string prefix_str{ prefix };
  auto first = std::lower_bound(range_begin, range_end, prefix_str);

  ++prefix_str.back();
  auto second = std::lower_bound(range_begin, range_end, prefix_str);

  return std::make_pair(first, second);
}

void TestFindStartsWith()
{
  {
    std::vector<std::string> sorted_strings{ "moscow", "motovilikha", "murmansk", "vologda" };
    auto result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");

    std::vector<std::string> expected{ "moscow", "motovilikha" };
    std::vector<std::string> results{ result.first, result.second };

    AssertEqual(expected, results, "");
  }

  {
    std::vector<std::string> sorted_strings{ "moscow", "moscow", "motovilikha", "motovilikha", "murmansk", "vologda" };
    auto result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");

    std::vector<std::string> expected{ "moscow", "moscow", "motovilikha", "motovilikha" };
    std::vector<std::string> results{ result.first, result.second };

    AssertEqual(expected, results, "");
  }

  {
    std::vector<std::string> sorted_strings{ "algebra", "solar", "solid", "temperature" };
    auto result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "sol");

    std::vector<std::string> expected{ "solar", "solid" };
    std::vector<std::string> results{ result.first, result.second };

    AssertEqual(expected, results, "");
  }

  {
    std::vector<std::string> sorted_strings{ "algorithm", "s", "s", "snake", "solar", "somnus", "temperature" };
    auto result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "sol");

    std::vector<std::string> expected{ "solar" };
    std::vector<std::string> results{ result.first, result.second };

    AssertEqual(expected, results, "");
  }

  {
    std::vector<std::string> sorted_strings{ "solar" };
    auto result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "so");

    std::vector<std::string> expected{ "solar" };
    std::vector<std::string> results{ result.first, result.second };

    AssertEqual(expected, results, "");
  }

  {
    std::vector<std::string> sorted_strings{ "moscow", "motovilikha", "murmansk" };
    auto result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");

    AssertEqual(result.first - sorted_strings.begin(), 2, "");
    AssertEqual(result.first - sorted_strings.begin(), result.second - sorted_strings.begin(), "");
  }

  {
    std::vector<std::string> sorted_strings{ "moscow", "motovilikha", "murmansk" };
    auto result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");

    AssertEqual(result.first - sorted_strings.begin(), 3, "");
    AssertEqual(result.first - sorted_strings.begin(), result.second - sorted_strings.begin(), "");
  }
}

void TestAll()
{
  TestRunner tr;
  tr.RunTest(TestFindStartsWith, "TestFindStartsWith");
}

int main()
{
  TestAll();

  const std::vector<std::string> sorted_strings = { "moscow", "motovilikha", "murmansk" };
  const auto m_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
  for (auto it = m_result.first; it != m_result.second; ++it)
  {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  return 0;
}
