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

/* присваивает r максимум из x и y */
#define MAX(x, y, r) do { int __x__ = (x); int __y__ = (y); r = __x__ > __y__ ? __x__ : __y__; } while(false)

int main() {
  int a = 10;
  int b = 20;
  int c = 0;

  MAX(a, b, c); // теперь c равно 20
  cout << c << endl;

  MAX(a += b, b, c); // теперь a = с = 30
  cout << c << endl;
  
  return 0;
}
