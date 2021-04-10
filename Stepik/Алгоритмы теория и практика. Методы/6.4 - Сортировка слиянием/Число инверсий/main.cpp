#include <iostream>
#include <vector>

std::vector<int> Merge(const std::vector<int>& first, const std::vector<int>& second, long& inversion_count)
{
  std::vector<int> result;
  result.reserve(first.size() + second.size());
  
  int f_idx = 0;
  int s_idx = 0;
  
  int first_size = static_cast<int>(first.size());
  int second_size = static_cast<int>(second.size());
  
  while (f_idx < first_size && s_idx < second_size)
  {
    if (first[f_idx] <= second[s_idx]) {
      result.push_back(first[f_idx++]);
    }
    else {
      result.push_back(second[s_idx++]);
      inversion_count += first_size - f_idx;
    }
  }
  
  while (f_idx < first_size) {
    result.push_back(first[f_idx++]);
  }
  
  while (s_idx < second_size) {
    result.push_back(second[s_idx++]);
  }

  return result;
}

void InversionCount(std::vector<int>& source_vec, long& inversion_count)
{
  if (source_vec.size() <= 1) {
    return;
  }
  
  int middle = source_vec.size() / 2;
  std::vector<int> left(source_vec.begin(), source_vec.begin() + middle);
  std::vector<int> right(source_vec.begin() + middle, source_vec.end());
  
  InversionCount(left, inversion_count);
  InversionCount(right, inversion_count);
  
  source_vec = Merge(left, right, inversion_count);
}

int main()
{
  int number_count;
  std::cin >> number_count;
  
  std::vector<int> numbers(number_count);
  for (int i = 0; i < number_count; ++i) {
    std::cin >> numbers[i];
  }
  
  long inversion_count = 0;
  InversionCount(numbers, inversion_count);
  std::cout << inversion_count;
  return 0;
}
