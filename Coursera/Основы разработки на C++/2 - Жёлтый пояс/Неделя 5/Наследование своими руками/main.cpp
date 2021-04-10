#include <iostream>

using namespace std;

class Animal {
public:
  Animal(const std::string & str) : Name(str)
  {}

  const string Name;
};


class Dog: public Animal {
public:
  Dog(const std::string & str) : Animal(str)
  {}

  void Bark() {
    cout << Name << " barks: woof!" << endl;
  }
};

int main()
{
  return 0;
}
