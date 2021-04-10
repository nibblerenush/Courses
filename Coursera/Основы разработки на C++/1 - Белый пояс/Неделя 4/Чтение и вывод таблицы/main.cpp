#include <fstream>
#include <iostream>
#include <iomanip>

int main()
{
  std::ifstream input("input.txt");
  if (input.is_open())
  {
    int N, M;
    input >> N >> M;
    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < M; ++j)
      {
        int value;
        input >> value;
        input.ignore(1);
        std::cout << std::setw(10) << value;
        if (j < M - 1)
        {
          std::cout << ' ';
        }
      }
      if (i < N - 1)
      {
        std::cout << std::endl;
      }
    }
  }
  return 0;
}
