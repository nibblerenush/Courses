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

// ���������� ������ ������� power, ���
//    x - �����, ������� ����� �������� � �������
//    p - �������, � ������� ����� �������� x
//

int power(int x, unsigned p) {
  int answer = 1;

  for (int i = 0; i < p; ++i) {
    answer *= x;
  }

  /* ������� answer */
  return answer;
}

int main() {
  cout << power(2, 4) << endl;
  return 0;
}
