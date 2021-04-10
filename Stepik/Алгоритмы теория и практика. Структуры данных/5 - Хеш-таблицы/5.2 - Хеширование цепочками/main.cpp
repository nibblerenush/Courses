#include <algorithm>
#include <cstdint>
#include <forward_list>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class HashTable
{
public:
  HashTable(std::size_t size) : m_vector(size)
  {}

  void Add(const std::string& str)
  {
    std::forward_list<std::string>& bucket = m_vector[HashFuncion(str)];
    if (std::find(bucket.begin(), bucket.end(), str) == bucket.end()) {
      bucket.push_front(str);
    }
  }

  void Del(const std::string& str)
  {
    std::forward_list<std::string>& bucket = m_vector[HashFuncion(str)];
    bucket.remove(str);
  }
  
  bool Find(const std::string& str) const
  {
    const std::forward_list<std::string>& bucket = m_vector[HashFuncion(str)];
    return std::find(bucket.begin(), bucket.end(), str) != bucket.end();
  }
  
  const std::forward_list<std::string>& Check(std::size_t index) const {
    return m_vector[index];
  }

public:
  std::size_t HashFuncion(const std::string& str) const
  {
    static const std::uint64_t x = 263;
    static const std::uint64_t p = 1'000'000'007;
    static const std::size_t m = m_vector.size();
    
    std::size_t result = 0;
    result += str[0] % p;
    
    std::uint64_t x_mod = x % p;
    std::uint64_t x_pow = x_mod;
    for (std::size_t i = 1; i < str.size(); ++i)
    {
      result += ((str[i] % p) * x_pow) % p;
      x_pow = (x_pow * x_mod) % p;
    }

    result = result % p % m;
    return result;
  }

private:
  std::vector<std::forward_list<std::string>> m_vector;
};

int main()
{
  std::size_t hash_table_size;
  std::size_t queries_count;
  std::cin >> hash_table_size >> queries_count;

  std::ostringstream output;
  HashTable hash_table(hash_table_size);

  for (std::size_t i = 0; i < queries_count; ++i)
  {
    std::string query;
    std::cin >> query;

    if (query == "add")
    {
      std::string str;
      std::cin >> str;
      hash_table.Add(str);
    }
    else if (query == "del")
    {
      std::string str;
      std::cin >> str;
      hash_table.Del(str);
    }
    else if (query == "find")
    {
      std::string str;
      std::cin >> str;
      if (hash_table.Find(str)) {
        output << "yes\n";
      }
      else {
        output << "no\n";
      }
    }
    else if (query == "check") {
      std::size_t index;
      std::cin >> index;
      const std::forward_list<std::string>& bucket = hash_table.Check(index);
      for (const auto& str : bucket) {
        output << str << ' ';
      }
      output << '\n';
    }
  }

  std::cout << output.str();
  return 0;
}
