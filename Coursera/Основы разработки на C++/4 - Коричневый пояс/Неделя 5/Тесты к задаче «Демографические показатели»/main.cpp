#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include "test_runner.h"

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

bool operator==(const Person& lhs, const Person& rhs) {
  return lhs.age == rhs.age
    && lhs.gender == rhs.gender
    && lhs.is_employed == rhs.is_employed;
}

ostream& operator<<(ostream& stream, const Person& person) {
  return stream << "Person(age=" << person.age
    << ", gender=" << static_cast<int>(person.gender)
    << ", is_employed=" << boolalpha << person.is_employed << ")";
}

struct AgeStats {
  int total;
  int females;
  int males;
  int employed_females;
  int unemployed_females;
  int employed_males;
  int unemployed_males;
};

bool operator == (const AgeStats& lhs, const AgeStats& rhs)
{
  return tie(lhs.total, lhs.females, lhs.males, lhs.employed_females, lhs.unemployed_females, lhs.employed_males, lhs.unemployed_males) ==
    tie(rhs.total, rhs.females, rhs.males, rhs.employed_females, rhs.unemployed_females, rhs.employed_males, rhs.unemployed_males);
}

bool operator != (const AgeStats& lhs, const AgeStats& rhs)
{
  return !(lhs == rhs);
}

ostream& operator << (ostream& output_stream, const AgeStats& age_stats)
{
  return output_stream;
}

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
  if (range_begin == range_end) {
    return 0;
  }
  vector<typename iterator_traits<InputIt>::value_type> range_copy(
    range_begin,
    range_end
    );
  auto middle = begin(range_copy) + range_copy.size() / 2;
  nth_element(
    begin(range_copy), middle, end(range_copy),
    [](const Person& lhs, const Person& rhs) {
    return lhs.age < rhs.age;
  }
  );
  return middle->age;
}

vector<Person> ReadPersons(istream& in_stream = cin) {
  int person_count;
  in_stream >> person_count;
  vector<Person> persons;
  persons.reserve(person_count);
  for (int i = 0; i < person_count; ++i) {
    int age, gender, is_employed;
    in_stream >> age >> gender >> is_employed;
    Person person{
        age,
        static_cast<Gender>(gender),
        is_employed == 1
    };
    persons.push_back(person);
  }
  return persons;
}

AgeStats ComputeStats(vector<Person> persons) {
  //                 persons
  //                |       |
  //          females        males
  //         |       |      |     |
  //      empl.  unempl. empl.   unempl.

  auto females_end = partition(
    begin(persons), end(persons),
    [](const Person& p) {
    return p.gender == Gender::FEMALE;
  }
  );
  auto employed_females_end = partition(
    begin(persons), females_end,
    [](const Person& p) {
    return p.is_employed;
  }
  );
  auto employed_males_end = partition(
    females_end, end(persons),
    [](const Person& p) {
    return p.is_employed;
  }
  );

  return {
       ComputeMedianAge(begin(persons), end(persons)),
       ComputeMedianAge(begin(persons), females_end),
       ComputeMedianAge(females_end, end(persons)),
       ComputeMedianAge(begin(persons),  employed_females_end),
       ComputeMedianAge(employed_females_end, females_end),
       ComputeMedianAge(females_end, employed_males_end),
       ComputeMedianAge(employed_males_end, end(persons))
  };
}

void PrintStats(const AgeStats& stats,
  ostream& out_stream = cout) {
  out_stream << "Median age = "
    << stats.total << endl
    << "Median age for females = "
    << stats.females << endl
    << "Median age for males = "
    << stats.males << endl
    << "Median age for employed females = "
    << stats.employed_females << endl
    << "Median age for unemployed females = "
    << stats.unemployed_females << endl
    << "Median age for employed males = "
    << stats.employed_males << endl
    << "Median age for unemployed males = "
    << stats.unemployed_males << endl;
}

void TestBug1() {
  vector<Person> empty;
  ASSERT(!ComputeMedianAge(empty.begin(), empty.end()));
}

void TestBug2() {
  vector<Person> persons = { {25, Gender::FEMALE, true}, {10, Gender::FEMALE, true}, {15, Gender::FEMALE, true} };
  ASSERT_EQUAL(ComputeMedianAge(persons.begin(), persons.end()), 15);
}

void TestBug3() {
  stringstream iss1;
  iss1 << 1 << ' ' << 25 << ' ' << 0 << ' ' << 1;
  vector<Person> expected1 = { {25, Gender::FEMALE, true} };
  ASSERT_EQUAL(ReadPersons(iss1), expected1);

  stringstream iss2;
  iss2 << 1 << ' ' << 25 << ' ' << 1 << ' ' << 0;
  vector<Person> expected2 = { {25, Gender::MALE, false} };
  ASSERT_EQUAL(ReadPersons(iss2), expected2);
}

void TestBug4() {
  stringstream ss1;
  ss1 << 1 << ' ' << 25 << ' ' << 0 << ' ' << 1;
  vector<Person> expected1 = { {25, Gender::FEMALE, true} };
  ASSERT_EQUAL(ReadPersons(ss1), expected1);

  stringstream ss2;
  ss2 << 1 << ' ' << 25 << ' ' << 0 << ' ' << 0;
  vector<Person> expected2 = { {25, Gender::FEMALE, false} };
  ASSERT_EQUAL(ReadPersons(ss2), expected2);
}

void TestBug5() {
  vector<Person> persons = { {25, Gender::MALE, true}, {10, Gender::MALE, true}, {15, Gender::MALE, true} };
  AgeStats ageStats = ComputeStats(persons);
  ASSERT_EQUAL(ageStats.employed_males, 15);
}

void TestBug6() {
  vector<Person> persons = {
    {40, Gender::FEMALE, true}, {30, Gender::FEMALE, true}, {20, Gender::FEMALE, false}, {10, Gender::FEMALE, false},
    {40, Gender::MALE, true}, {30, Gender::MALE, true}, {20, Gender::MALE, false}, {10, Gender::MALE, false}
  };

  AgeStats stats = ComputeStats(persons);
  AgeStats expected_age_stats;
  expected_age_stats.total = 30;
  expected_age_stats.females = 30;
  expected_age_stats.males = 30;
  expected_age_stats.employed_females = 40;
  expected_age_stats.unemployed_females = 20;
  expected_age_stats.employed_males = 40;
  expected_age_stats.unemployed_males = 20;
  ASSERT_EQUAL(stats, expected_age_stats);

  ostringstream oss;
  PrintStats(stats, oss);

  ostringstream expected_oss;
  expected_oss << "Median age = "
    << stats.total << endl
    << "Median age for females = "
    << stats.females << endl
    << "Median age for males = "
    << stats.males << endl
    << "Median age for employed females = "
    << stats.employed_females << endl
    << "Median age for unemployed females = "
    << stats.unemployed_females << endl
    << "Median age for employed males = "
    << stats.employed_males << endl
    << "Median age for unemployed males = "
    << stats.unemployed_males << endl;
  ASSERT_EQUAL(oss.str(), expected_oss.str());
}

int main() {
  //PrintStats(ComputeStats(ReadPersons()));
  TestRunner tr;
  RUN_TEST(tr, TestBug1);
  RUN_TEST(tr, TestBug2);
  RUN_TEST(tr, TestBug3);
  RUN_TEST(tr, TestBug4);
  RUN_TEST(tr, TestBug5);
  RUN_TEST(tr, TestBug6);
  return 0;
}
