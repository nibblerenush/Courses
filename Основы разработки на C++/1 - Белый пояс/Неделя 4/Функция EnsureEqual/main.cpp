#include <exception>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void EnsureEqual(const string& left, const string& right)
{
  if (left != right)
  {
    std::ostringstream oss;
    oss << left << " != " << right;
    throw std::runtime_error(oss.str());
  }
}

int main() {
  try {
    EnsureEqual("C++ White", "C++ White");
    EnsureEqual("C++ White", "C++ Yellow");
  }
  catch (runtime_error& e) {
    cout << e.what() << endl;
  }
  return 0;
}