#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

bool is_greater_or_equal(const string& num, const string& max_num) {
  using namespace std;

  string x = num + max_num;
  string y = max_num + num;

  return x > y;
}

string largest_number(vector<string> a) {
  std::stringstream ret;

  while (!a.empty()) {
    std::string max_num = a[0];
    int max_index = 0;

    for (int i = 1; i < (int)a.size(); ++i) {
      if (is_greater_or_equal(a[i], max_num)) {
        max_num = a[i];
        max_index = i;
      }
    }

    ret << a[max_index];
    swap(a[max_index], a.back());
    a.pop_back();
  }
  
  string result;
  ret >> result;
  return result;
}

int main() {
  int n;
  std::cin >> n;
  vector<string> a(n);
  for (size_t i = 0; i < a.size(); i++) {
    std::cin >> a[i];
  }
  std::cout << largest_number(a);
}
