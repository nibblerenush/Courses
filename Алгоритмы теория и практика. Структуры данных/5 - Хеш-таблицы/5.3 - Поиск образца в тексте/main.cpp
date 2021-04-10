#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

static const std::int64_t x = 263;
static const std::int64_t p = 1'000'000'007;

std::int64_t HashFunction(const std::string& str, std::size_t begin, std::size_t end)
{
  std::int64_t result = 0;
  result += str[begin] % p;
  
  std::int64_t x_mod = x % p;
  std::int64_t x_pow = x_mod;
  for (std::size_t i = begin + 1; i <= end; ++i)
  {
    result += ((str[i] % p) * x_pow) % p;
    x_pow = (x_pow * x_mod) % p;
  }
  
  result %= p;
  return result;
}

std::int64_t PatternConst(const std::string& pattern)
{
  std::int64_t result = 1;
  std::int64_t x_mod = x % p;
  std::int64_t x_pow = x_mod;
  for (std::size_t i = 1; i < pattern.length(); ++i)
  {
    result = x_pow;
    x_pow = (x_pow * x_mod) % p;
  }
  return result;
}

int main()
{
  std::string pattern;
  std::string text;
  std::cin >> pattern >> text;
  
  std::int64_t hash_value_of_pattern = HashFunction(pattern, 0, pattern.length() - 1);
  std::int64_t pattern_const = PatternConst(pattern);
  
  std::vector<std::int64_t> hash_values_of_text_part(text.length() - pattern.length() + 1);
  std::size_t result_size = hash_values_of_text_part.size();
  
  hash_values_of_text_part[result_size - 1] = HashFunction(text, result_size - 1, text.length() - 1);
  
  for (int i = static_cast<int>(result_size) - 2; i >= 0; --i)
  {
    std::int64_t part1 = hash_values_of_text_part[i + 1] % p;
    std::int64_t part2 = ((text[i + pattern.length()] % p) * pattern_const) % p;

    std::int64_t part12 = part1 - part2;
    part12 = part12 >= 0 ? part12 : part12 % p + p;
    
    std::int64_t part3 = ((part12 % p) * (x % p)) % p;
    hash_values_of_text_part[i] = (part3 + text[i] % p) % p;
  }

  for (std::size_t i = 0; i < result_size; ++i)
  {
    if (hash_values_of_text_part[i] == hash_value_of_pattern)
    {
      bool is_substr = true;
      for (std::size_t str_i = i; str_i < i + pattern.length(); ++str_i)
      {
        if (text[str_i] != pattern[str_i - i])
        {
          is_substr = false;
          break;
        }
      }
      if (is_substr) {
        std::cout << i << ' ';
      }
    }
  }

  return 0;
}
