#include "test_runner.h"

#include <iostream>
#include <deque>
#include <string>

void TestAll()
{
  TestRunner tr;
}

bool BiggerPriority(const char& old_op, const char& op)
{
  return ((old_op == '+' || old_op == '-') && (op == '*' || op == '/'));
}

int main()
{
  TestAll();
  
  int x;
  std::cin >> x;
  std::deque<std::string> task_deque;
  task_deque.push_back(std::to_string(x));

  int N;
  std::cin >> N;
  char old_operation;
  for (int counter = 0; counter < N; ++counter)
  {
    char operation;
    int number;
    std::cin >> operation >> number;
    
    if (task_deque.size() == 1)
    {
      task_deque.push_back({' ', operation, ' ' });
      task_deque.push_back(std::to_string(number));
    }
    else
    {
      if (BiggerPriority(old_operation, operation))
      {
        task_deque.push_front("(");
        task_deque.push_back({ ')', ' ', operation, ' ' });
        task_deque.push_back(std::to_string(number));
      }
      else
      {
        task_deque.push_back({ ' ', operation, ' ' });
        task_deque.push_back(std::to_string(number));
      }
    }

    old_operation = operation;
  }
  for (const auto& part : task_deque)
  {
    std::cout << part;
  }
  return 0;
}
