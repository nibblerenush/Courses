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

int main() {
  int a, b, c;
  cin >> a >> b >> c;

  int D = b * b - 4 * a * c;
  if (D < 0) {
    cout << "No real roots" << endl;
  }
  else {
    double x1 = (-b + sqrt(D)) / (2 * a);
    double x2 = (-b - sqrt(D)) / (2 * a);
    cout << x1 << ' ' << x2 << endl;
  }

  return 0;
}
