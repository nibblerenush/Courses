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

int main()
{
  char c = '\0';
  bool first_space = true;
  
  while (cin.get(c)) {
    if (c != ' ') {
      cout << c;
      first_space = true;
    }
    else if (first_space) {
      cout << c;
      first_space = false;
    }
  }

  return 0;
}
