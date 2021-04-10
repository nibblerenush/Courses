#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <cmath>
#include <future>
#include <numeric>
#include <random>
#include <string>
#include <vector>
using namespace std;

template <typename K, typename V>
class ConcurrentMap {
public:
  static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");
  
  struct Access
  {
    std::lock_guard<std::mutex> guard;
    V& ref_to_value;
  };

  explicit ConcurrentMap(size_t bucket_count);

  Access operator[](const K& key)
  {
    std::size_t index;
    int s_index = key % m_map_vec.size();
    if (s_index < 0)
    {
      index = -s_index;
    }
    else
    {
      index = s_index;
    }

    return Access{ std::lock_guard(m_mutex_vec[index]), m_map_vec[index][key] };
  }

  map<K, V> BuildOrdinaryMap();

private:
  std::vector<std::mutex> m_mutex_vec;
  std::vector<std::map<K, V>> m_map_vec;
};

template <typename K, typename V>
ConcurrentMap<K, V>::ConcurrentMap(std::size_t bucket_count) : m_mutex_vec(bucket_count), m_map_vec(bucket_count)
{}

//template <typename K, typename V>
//typename ConcurrentMap<K, V>::Access ConcurrentMap<K, V>::operator[](const K& key)
//{}

template <typename K, typename V>
std::map<K, V> ConcurrentMap<K, V>::BuildOrdinaryMap()
{
  std::map<K, V> result;
  for (std::size_t i = 0; i < m_map_vec.size(); ++i)
  {
    std::lock_guard<std::mutex> guard(m_mutex_vec[i]);
    result.insert(m_map_vec[i].begin(), m_map_vec[i].end());
  }
  return result;
}

void RunConcurrentUpdates(
  ConcurrentMap<int, int>& cm, size_t thread_count, int key_count
  ) {
  auto kernel = [&cm, key_count](int seed) {
    vector<int> updates(key_count);
    iota(begin(updates), end(updates), -key_count / 2);
    shuffle(begin(updates), end(updates), default_random_engine(seed));

    for (int i = 0; i < 2; ++i) {
      for (auto key : updates) {
        cm[key].ref_to_value++;
      }
    }
  };

  vector<future<void>> futures;
  for (size_t i = 0; i < thread_count; ++i) {
    futures.push_back(async(kernel, i));
  }
}

void TestConcurrentUpdate() {
  const size_t thread_count = 3;
  const size_t key_count = 50000;

  ConcurrentMap<int, int> cm(thread_count);
  RunConcurrentUpdates(cm, thread_count, key_count);

  {
    const auto result = cm.BuildOrdinaryMap();
    ASSERT_EQUAL(result.size(), key_count);
    for (auto& [k, v] : result) {
      AssertEqual(v, 6, "Key = " + to_string(k));
    }
  }  
}

void TestReadAndWrite() {
  ConcurrentMap<size_t, string> cm(5);

  auto updater = [&cm] {
    for (size_t i = 0; i < 50000; ++i) {
      cm[i].ref_to_value += 'a';
    }
  };
  auto reader = [&cm] {
    vector<string> result(50000);
    for (size_t i = 0; i < result.size(); ++i) {
      result[i] = cm[i].ref_to_value;
    }
    return result;
  };

  auto u1 = async(updater);
  auto r1 = async(reader);
  auto u2 = async(updater);
  auto r2 = async(reader);

  u1.get();
  u2.get();

  for (auto f : { &r1, &r2 }) {
    auto result = f->get();
    ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
      return s.empty() || s == "a" || s == "aa";
    }));
  }
}

void TestSpeedup() {
  {
    ConcurrentMap<int, int> single_lock(1);

    LOG_DURATION("Single lock");
    RunConcurrentUpdates(single_lock, 4, 50000);
  }
  {
    ConcurrentMap<int, int> many_locks(100);

    LOG_DURATION("100 locks");
    RunConcurrentUpdates(many_locks, 4, 50000);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestConcurrentUpdate);
  RUN_TEST(tr, TestReadAndWrite);
  RUN_TEST(tr, TestSpeedup);
}
