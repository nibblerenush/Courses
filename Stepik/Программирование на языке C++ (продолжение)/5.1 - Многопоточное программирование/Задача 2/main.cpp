#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename Iter, typename UnaryFunc, typename BinaryFunc>
auto map_reduce_helper(Iter first, Iter last, UnaryFunc unary_func, BinaryFunc binary_func) ->
decltype(binary_func(unary_func(*first), unary_func(*first)))
{
  using result_type = decltype(binary_func(unary_func(*first), unary_func(*first)));

  int length = std::distance(first, last);
  if (length == 0) {
    return result_type{};
  }

  result_type result = unary_func(*first);
  while (++first != last) {
    result = binary_func(result, unary_func(*first));
  }

  return result;
}

template <typename Iter, typename UnaryFunc, typename BinaryFunc>
auto map_reduce(Iter first, Iter last, UnaryFunc unary_func, BinaryFunc binary_func, std::size_t num) ->
decltype(binary_func(unary_func(*first), unary_func(*first)))
{
  using result_type = decltype(binary_func(unary_func(*first), unary_func(*first)));

  int length = std::distance(first, last);
  if (length == 0 || num == 0) {
    return result_type{};
  }

  int part_length = length / num;
  std::vector<std::thread> threads;
  std::vector<result_type> results(num);

  for (std::size_t i = 0; i < num - 1; ++i) {
    Iter old_first = first;
    std::advance(first, part_length);
    threads.push_back(
      std::thread{
        [&results, i, old_first, first, unary_func, binary_func]
        () { results[i] = map_reduce_helper<Iter, UnaryFunc, BinaryFunc>(old_first, first, unary_func, binary_func); } });
  }
  threads.push_back(
    std::thread{
      [&results, num, first, last, unary_func, binary_func]
      () { results[num - 1] = map_reduce_helper<Iter, UnaryFunc, BinaryFunc>(first, last, unary_func, binary_func); } });
  
  for (std::size_t i = 0; i < num; ++i) {
    if (threads[i].joinable()) {
      threads[i].join();
    }
  };

  result_type result = results[0];
  for (std::size_t i = 1; i < num; ++i) {
    result = binary_func(result, results[i]);
  }

  return result;
}

int main() {
  std::list<int> l = { 1,2,3,4,5,6,7,8,9,10 };
  // параллельное суммирование в 3 потока
  auto sum = map_reduce(l.begin(), l.end(),
    [](int i) {return i; },
    std::plus<int>(), 3);

  std::cout << sum << std::endl;

  // проверка наличия чётных чисел в четыре потока
  auto has_even = map_reduce(l.begin(), l.end(),
    [](int i) {return i % 2 == 0; },
    std::logical_or<bool>(), 4);

  std::cout << std::boolalpha << has_even << std::endl;

  return 0;
}
