#include "animals.h"
#include "test_runner.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

using Zoo = vector<unique_ptr<Animal>>;

// ��� ������� �������� �� ���� ����� ����� � ������ �� ���� �������� ������.
// ���� ��������� ����� ����� ������ - Tiger, Wolf ��� Fox, ������� ������ ��������� ���������������� ����� � �������.
// � ��������� ������ ��� ������ ���������� ������ � ������������� ���������� runtime_error.
Zoo CreateZoo(istream& in) {
  Zoo zoo;
  string word;
  while (in >> word) {
    if (word == "Tiger") {
      zoo.push_back(make_unique<Tiger>());
    }
    else if (word == "Wolf") {
      zoo.push_back(make_unique<Wolf>());
    }
    else if (word == "Fox") {
      zoo.push_back(make_unique<Fox>());
    }
    else {
      throw runtime_error("Unknown animal!");
    }
  }
  return zoo;
}

// ��� ������� ������ ��������� ���� ������ � �������� � ������� �� ��������
// � �������� � �������� ����� ��� ������� �� ��� ��������� ������ ����������� ������� voice.
// ���������� ������ ������ ������ �������� �������� ������ '\n'.
void Process(const Zoo& zoo, ostream& out) {
  for (const auto& animal : zoo) {
    out << animal->Voice() << "\n";
  }
}

void TestZoo() {
  istringstream input("Tiger Wolf Fox Tiger");
  ostringstream output;
  Process(CreateZoo(input), output);

  const string expected =
    "Rrrr\n"
    "Wooo\n"
    "Tyaf\n"
    "Rrrr\n";

  ASSERT_EQUAL(output.str(), expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestZoo);
}
