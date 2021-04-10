#include <iostream>
#include <vector>

int main()
{
  int input_numbers_count;
  std::cin >> input_numbers_count;

  std::vector<int> numbers(input_numbers_count);
  std::vector<int> number_count(11, 0);
  
  for (int i = 0; i < input_numbers_count; ++i)
  {
    std::cin >> numbers[i];
    number_count[numbers[i]]++;
  }

  for (std::size_t i = 2; i < number_count.size(); ++i)
  {
    number_count[i] += number_count[i - 1];
  }
  
  std::vector<int> result(input_numbers_count);
  for (int i = input_numbers_count - 1; i >= 0; --i)
  {
    result[number_count[numbers[i]] - 1] = numbers[i];
    number_count[numbers[i]]--;
  }

  for (int i = 0; i < input_numbers_count; ++i)
  {
    std::cout << result[i] << ' ';
  }

  return 0;
}
