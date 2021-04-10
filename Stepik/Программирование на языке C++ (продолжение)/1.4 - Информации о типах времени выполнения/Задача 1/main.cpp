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
#include <typeinfo>

using namespace std;

class Expression {};

bool check_equals(Expression const* left, Expression const* right) {
  const Expression& left_ref = *left;
  const Expression& right_ref = *right;
  return typeid(left_ref) == typeid(right_ref);
}

int main() {
  return 0;
}
