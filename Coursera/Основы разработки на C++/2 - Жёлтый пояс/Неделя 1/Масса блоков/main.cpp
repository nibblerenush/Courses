#include <iostream>
#include <vector>

int main()
{
  std::uint32_t N;
  std::cin >> N;
  
  std::uint32_t R;
  std::cin >> R;

  std::uint64_t result{0};
  for (std::uint32_t i = 0; i < N; ++i)
  {
    std::uint64_t a, b, c;
    std::cin >> a >> b >> c;
    std::uint64_t m = a * b * c * R;
    result += m;
  }
  std::cout << result;
  return 0;
}
