#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct Foo {
  void say() const { std::cout << "Foo says: " << msg << "\n"; }
protected:
  Foo(const char* msg) : msg(msg) { }
private:
  const char* msg;
};

void foo_says(const Foo& foo) { foo.say(); }

struct DFoo : public Foo {
public:
  DFoo(const char* msg) : Foo(msg)
  {}
};

DFoo get_foo(const char* msg) {
  return DFoo(msg);
}

int main() {
  const char* msg = "Hello World!";

  foo_says(get_foo(msg));
  return 0;
}
