#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

enum class Gender {
  FEMALE,
  MALE
};

struct Person {
  int age;
  Gender gender;
  bool is_employed;
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
  if (range_begin == range_end) {
    return 0;
  }
  vector<typename InputIt::value_type> range_copy(range_begin, range_end);
  auto middle = begin(range_copy) + range_copy.size() / 2;
  nth_element(
    begin(range_copy), middle, end(range_copy),
    [](const Person& lhs, const Person& rhs) {
    return lhs.age < rhs.age;
  }
  );
  return middle->age;
}

template <typename InputIt>
void PrintMedian(InputIt range_begin, InputIt range_end, const std::string& info)
{
  int median = ComputeMedianAge(range_begin, range_end);
  std::cout << info << " = " << median << std::endl;
}

void PrintStats(vector<Person> persons)
{
  PrintMedian(persons.begin(), persons.end(), "Median age");

  PrintMedian(persons.begin(),
    partition(persons.begin(), persons.end(), [](const auto& p) { return p.gender == Gender::FEMALE; }),
    "Median age for females");

  PrintMedian(persons.begin(),
    partition(persons.begin(), persons.end(), [](const auto& p) { return p.gender == Gender::MALE; }),
    "Median age for males");

  PrintMedian(persons.begin(),
    partition(persons.begin(), persons.end(), [](const auto& p) { return p.gender == Gender::FEMALE && p.is_employed; }),
    "Median age for employed females");

  PrintMedian(persons.begin(),
    partition(persons.begin(), persons.end(), [](const auto& p) { return p.gender == Gender::FEMALE && !p.is_employed; }),
    "Median age for unemployed females");

  PrintMedian(persons.begin(),
    partition(persons.begin(), persons.end(), [](const auto& p) { return p.gender == Gender::MALE && p.is_employed; }),
    "Median age for employed males");

  PrintMedian(persons.begin(),
    partition(persons.begin(), persons.end(), [](const auto& p) { return p.gender == Gender::MALE && !p.is_employed; }),
    "Median age for unemployed males");
}

int main() {
  vector<Person> persons = {
      {31, Gender::MALE, false},
      {40, Gender::FEMALE, true},
      {24, Gender::MALE, true},
      {20, Gender::FEMALE, true},
      {80, Gender::FEMALE, false},
      {78, Gender::MALE, false},
      {10, Gender::FEMALE, false},
      {55, Gender::MALE, true},
  };
  PrintStats(persons);
  return 0;
}

