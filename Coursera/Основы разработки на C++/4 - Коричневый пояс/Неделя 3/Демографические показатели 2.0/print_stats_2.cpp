#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
  {}

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

private:
  Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
  return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person {
  string name;
  int age, income;
  bool is_male;
};

vector<Person> ReadPeople(istream& input)
{
  int count;
  input >> count;
  
  vector<Person> result(count);
  for (Person& p : result)
  {
    char gender;
    input >> p.name >> p.age >> p.income >> gender;
    p.is_male = gender == 'M';
  }

  return result;
}

int main()
{
  int temp_index;
  const vector<Person> people = [&temp_index]() {
    vector<Person> result = ReadPeople(cin);
    auto iter = partition(result.begin(), result.end(), [](const Person& person) {
      return person.is_male;
    });
    sort(result.begin(), iter, [](const Person& lhs, const Person& rhs) {
      return lhs.name < rhs.name;
    });
    sort(iter, result.end(), [](const Person& lhs, const Person& rhs) {
      return lhs.name < rhs.name;
    });
    temp_index = distance(result.begin(), iter);
    return result;
  }();

  auto get_name = [] (IteratorRange<vector<Person>::const_iterator> gender_people) {
    if (gender_people.begin() == gender_people.end()) {
      return string();
    }
    else {
      int count = 1;
      const string* most_popular_name = &gender_people.begin()->name;
      for (auto i = gender_people.begin(); i != gender_people.end(); )
      {
        auto same_name_end = find_if_not(i, gender_people.end(), [i](const Person& p) {
          return p.name == i->name;
        });
        auto cur_name_count = distance(i, same_name_end);
        if (cur_name_count > count) {
          count = cur_name_count;
          most_popular_name = &i->name;
        }
        i = same_name_end;
      }
      return string(*most_popular_name);
    }
  };

  IteratorRange males(people.begin(), people.begin() + temp_index);
  string male_name = get_name(males);
  
  IteratorRange females(people.begin() + temp_index, people.end());
  string female_name = get_name(females);

  const vector<int> sorted_age = [&people]() {
    vector<int> result(people.size());
    for (size_t i = 0; i < people.size(); ++i) {
      result[i] = people[i].age;
    }
    sort(result.begin(), result.end());
    return result;
  }();

  const vector<int> sorted_income = [&people] {
    vector<int> result(people.size());
    for (size_t i = 0; i < people.size(); ++i) {
      result[i] = people[i].income;
    }
    sort(result.rbegin(), result.rend(), [](int lhs, int rhs) { return lhs < rhs; });
    return result;
  }();

  for (string command; cin >> command; )
  {
    if (command == "AGE")
    {
      int adult_age; cin >> adult_age;
      auto adult_begin = lower_bound(sorted_age.begin(), sorted_age.end(), adult_age);
      cout << "There are " << distance(adult_begin, end(sorted_age)) << " adult people for maturity age " << adult_age << '\n';
    }
    else if (command == "WEALTHY")
    {
      int count; cin >> count;
      auto head = Head(sorted_income, count);
      int total_income = accumulate(head.begin(), head.end(), 0);
      cout << "Top-" << count << " people have total income " << total_income << '\n';
    }
    else if (command == "POPULAR_NAME")
    {
      char gender; cin >> gender;
      string gender_name = gender == 'M' ? male_name : female_name;
      if (gender_name.empty()) {
        cout << "No people of gender " << gender << '\n';
      }
      else {
        cout << "Most popular name among people of gender " << gender << " is " << gender_name << '\n';
      }
    }
  }
}
