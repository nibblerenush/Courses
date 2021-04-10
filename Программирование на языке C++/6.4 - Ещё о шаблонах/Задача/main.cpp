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

template <typename T, size_t size>
size_t array_size(T(&arr)[size]) {
  return size;
}

int main() {
  int ints[] = { 1, 2, 3, 4 };
  int* iptr = ints;
  double doubles[] = { 3.14 };
  cout << array_size(ints) << endl; // вернет 4
  cout << array_size(doubles) << endl; // вернет 1
  //array_size(iptr); // тут должна произойти ошибка компиляции
  return 0;
}
