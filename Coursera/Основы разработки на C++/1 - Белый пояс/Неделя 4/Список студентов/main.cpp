#include <iostream>
#include <string>
#include <vector>

struct Student
{
  std::string name;
  std::string surname;
  int day;
  int month;
  int year;
};

int main()
{
  int N;
  std::cin >> N;
  std::vector<Student> students;
  for (int counter = 0; counter < N; ++counter)
  {
    Student student;
    std::cin >> student.name >> student.surname >> student.day >> student.month >> student.year;
    students.push_back(student);
  }

  int M;
  std::cin >> M;
  for (int counter = 0; counter < M; ++counter)
  {
    std::string command;
    std::cin >> command;

    if (command == "name")
    {
      int K;
      std::cin >> K;

      if (K > 0 && students.size() > 0 && K <= students.size())
      {
        std::cout << students[K - 1].name << ' ' << students[K - 1].surname << std::endl;
      }
      else
      {
        std::cout << "bad request" << std::endl;
      }
    }
    else if (command == "date")
    {
      int K;
      std::cin >> K;

      if (K > 0 && students.size() > 0 && K <= students.size())
      {
        std::cout << students[K - 1].day << '.' << students[K - 1].month << '.' << students[K - 1].year << std::endl;
      }
      else
      {
        std::cout << "bad request" << std::endl;
      }
    }
    else
    {
      int K;
      std::cin >> K;
      std::cout << "bad request" << std::endl;
    }
  }

  return 0;
}
