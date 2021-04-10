#include "test_runner.h"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

// Объявляем Group<String> для произвольного типа String
// синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template <typename String>
using Group = vector<String>;

// Ещё один шаблонный синоним типа
// позволяет вместо громоздкого typename String::value_type
// использовать Char<String>
template <typename String>
using Char = typename String::value_type;

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings)
{
  std::vector<Group<String>> result;
  std::map<std::set<Char<String>>, int> set_index;

  for (auto& str : strings)
  {
    std::set<Char<String>> set_key;
    for (auto& c : str)
    {
      set_key.insert(std::move(c));
    }

    auto iter = set_index.find(set_key);
    if (iter == set_index.end())
    {
      result.push_back(Group<String>{std::move(str)});
      set_index[std::move(set_key)] = result.size() - 1;
    }
    else
    {
      result[iter->second].push_back(std::move(str));
    }
  }

  return result;
}


void TestGroupingABC() {
  vector<string> strings = { "caab", "abc", "cccc", "bacc", "c" };
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 2);
  sort(begin(groups), end(groups));  // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({ "caab", "abc", "bacc" }));
  ASSERT_EQUAL(groups[1], vector<string>({ "cccc", "c" }));
}

void TestGroupingReal() {
  vector<string> strings = { "law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl" };
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 4);
  sort(begin(groups), end(groups));  // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({ "laptop", "paloalto" }));
  ASSERT_EQUAL(groups[1], vector<string>({ "law", "wall", "awl" }));
  ASSERT_EQUAL(groups[2], vector<string>({ "port" }));
  ASSERT_EQUAL(groups[3], vector<string>({ "top", "pot" }));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestGroupingABC);
  RUN_TEST(tr, TestGroupingReal);
  return 0;
}
