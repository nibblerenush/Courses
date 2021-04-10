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

struct Cls {
  Cls(char c, double d, int i) : c(c), d(d), i(i)
  {}

private:
  char c;
  double d;
  int i;
};

// Эта функция должна предоставить доступ к полю c объекта cls.
// Обратите внимание, что возвращается ссылка на char, т. е.
// доступ предоставляется на чтение и запись.
char& get_c(Cls& cls) {
  char* data = reinterpret_cast<char*>(&cls);
  return *data;
}

// Эта функция должна предоставить доступ к полю d объекта cls.
// Обратите внимание, что возвращается ссылка на double, т. е.
// доступ предоставляется на чтение и запись.
double& get_d(Cls& cls) {
  double* data = reinterpret_cast<double*>(&cls);
  data += 1;
  return *data;
}

// Эта функция должна предоставить доступ к полю i объекта cls.
// Обратите внимание, что возвращается ссылка на int, т. е.
// доступ предоставляется на чтение и запись.
int& get_i(Cls& cls) {
  double* data = reinterpret_cast<double*>(&cls);
  data += 2;
  return *reinterpret_cast<int*>(data);
}

int main() {
  Cls cls('a', 1.5, 3);

  get_c(cls) = 'b';
  get_d(cls) = 3.5;
  get_i(cls) = 5;

  return 0;
}
