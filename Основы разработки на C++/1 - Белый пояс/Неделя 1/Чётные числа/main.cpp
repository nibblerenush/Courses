#include <iostream>

int main()
{
  int A, B;
  std::cin >> A >> B;

  int startNumber;
  if (A % 2 == 0)
  {
    startNumber = A;
  }
  else
  {
    startNumber = A + 1;
  }

  for (int i = startNumber; i <= B; i += 2)
  {
    std::cout << i << ' ';
  }

  return 0;
}
