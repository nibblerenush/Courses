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

char* getline() {
  int str_size = 3;
  char* str = new char[str_size];
  
  char c = 0;
  int get_size = 0;
  int get_index = -1;
  
  while (cin.get(c) && c != '\n') {
    get_size += 1;
    get_index += 1;
    
    if (get_size == str_size) {
      char* new_str = new char[2 * str_size];
      copy(str, str + str_size, new_str);
      delete[] str;

      str_size *= 2;
      str = new_str;
    }

    str[get_index] = c;
  }

  str[get_size] = '\0';
  return str;
}

int main() {
  char* str = getline();
  cout << str << endl;
  return 0;
}
