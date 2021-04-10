#include "test_runner.h"

template <typename T, typename U>
bool Helper(const T& compare, const U& value) {
  return compare == value;
}

template <typename T, typename U, typename ... Args>
bool Helper(const T& compare, const U& value, const Args& ... args) {
  return compare == value || Helper(compare, args...);
}

template <typename T>
bool EqualsToOneOf(const T& compare) {
  return false;
}

template <typename T, typename ... Args>
bool EqualsToOneOf(const T& compare, const Args& ... args) {
  return Helper(compare, args...);
}

void Test() {
  auto x = "pear";
  ASSERT(EqualsToOneOf(x, "pear"));
  ASSERT(!EqualsToOneOf(x, "apple"));
  ASSERT(EqualsToOneOf(x, "apple", "pear"));
  ASSERT(!EqualsToOneOf(x, "apple", "banana"));
  ASSERT(EqualsToOneOf(x, "apple", "banana", "pear"));
  ASSERT(!EqualsToOneOf(x, "apple", "banana", "peach"));
  ASSERT(EqualsToOneOf(x, "apple", "banana", "pear", "orange"));
  ASSERT(!EqualsToOneOf(x, "apple", "banana", "peach", "orange"));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);
  return 0;
}
