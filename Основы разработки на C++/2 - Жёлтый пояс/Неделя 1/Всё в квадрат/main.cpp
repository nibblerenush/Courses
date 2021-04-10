#include <iostream>
#include <map>
#include <utility>
#include <vector>

template<typename T>
T Sqr(const T& x);

template<typename T>
std::vector<T> Sqr(const std::vector<T>& vector);

template<typename Key, typename Value>
std::map<Key, Value> Sqr(const std::map<Key, Value>& dict);

template<typename First, typename Second>
std::pair<First, Second> Sqr(const std::pair<First, Second>& pr);

template<typename T>
T Sqr(const T& x)
{
  return x * x;
}

template<typename T>
std::vector<T> Sqr(const std::vector<T>& vect)
{
  std::vector<T> result;
  for (const auto& value : vect)
  {
    result.push_back(Sqr(value));
  }
  return result;
}

template<typename Key, typename Value>
std::map<Key, Value> Sqr(const std::map<Key, Value>& dict)
{
  std::map<Key, Value> result;
  for (const auto& item : dict)
  {
    result[item.first] = Sqr(item.second);
  }
  return result;
}

template<typename First, typename Second>
std::pair<First, Second> Sqr(const std::pair<First, Second>& pr)
{
  return { Sqr(pr.first), Sqr(pr.second) };
}

int main()
{
  std::vector<int> v = { 1, 2, 3 };
  std::cout << "vector:";
  for (int x : Sqr(v)) {
    std::cout << ' ' << x;
  }
  std::cout << std::endl;

  std::map<std::string, int> m = { {"one", 1}, {"two", 2}, {"three", 3} };
  std::cout << "map:";
  for (const auto& item : Sqr(m))
  {
    std::cout << ' ' << item.first << ": " << item.second;
  }

  std::map<int, std::pair<int, int>> map_of_pairs = {
    {4, {2, 2}},
    {7, {4, 3}}
  };
  std::cout << "map of pairs:" << std::endl;
  for (const auto& x : Sqr(map_of_pairs)) {
    std::cout << x.first << ' ' << x.second.first << ' ' << x.second.second << std::endl;
  }

  return 0;
}
