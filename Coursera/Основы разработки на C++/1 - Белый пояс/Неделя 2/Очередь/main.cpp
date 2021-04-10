#include <algorithm>
#include <iostream>
#include <vector>

void AddPersons(std::vector<int>& queue, int k)
{
  for (int i = 0; i < k; ++i)
  {
    queue.push_back(0);
  }
}

void RemovePersons(std::vector<int>& queue, int k)
{
  for (int i = 0; i < k; ++i)
  {
    queue.pop_back();
  }
}

int main()
{
  std::vector<int> queue;

  int Q;
  std::cin >> Q;

  const std::string WORRY = "WORRY";
  const std::string QUIET = "QUIET";
  const std::string COME = "COME";
  const std::string WORRY_COUNT = "WORRY_COUNT";

  for (int i = 0; i < Q; ++i)
  {
    std::string command;
    std::cin >> command;

    if (command == WORRY)
    {
      int index;
      std::cin >> index;
      queue[index] = 1;
    }
    else if (command == QUIET)
    {
      int index;
      std::cin >> index;
      queue[index] = 0;
    }
    else if (command == COME)
    {
      int count_of_persons;
      std::cin >> count_of_persons;
      if (count_of_persons > 0)
      {
        AddPersons(queue, count_of_persons);
      }
      else if (count_of_persons < 0)
      {
        RemovePersons(queue, -count_of_persons);
      }
    }
    else if (command == WORRY_COUNT)
    {
      std::cout << std::count(std::begin(queue), std::end(queue), 1) << std::endl;
    }
  }

  return 0;
}
