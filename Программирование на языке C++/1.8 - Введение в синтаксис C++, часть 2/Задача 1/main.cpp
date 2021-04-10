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

// определите только функцию power, где
//    x - число, которое нужно возвести в степень
//    p - степень, в которую нужно возвести x
//

int power(int x, unsigned p) {
  int answer = 1;

  for (int i = 0; i < p; ++i) {
    answer *= x;
  }

  /* считаем answer */
  return answer;
}

int main() {
  cout << power(2, 4) << endl;
  return 0;
}
