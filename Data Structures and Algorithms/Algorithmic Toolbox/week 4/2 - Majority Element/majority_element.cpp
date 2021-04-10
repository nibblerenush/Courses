#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

int get_majority_element(vector<int> &a, int left, int right) {
  if (left == right) return -1;
  if (left + 1 == right) return a[left];

  int mid = left + (right - left) / 2;
  int left_maj = get_majority_element(a, left, mid);
  int right_maj = get_majority_element(a, mid, right);

  if (left_maj == right_maj) {
    return left_maj;
  }
  else {
    int left_count = 0;
    int right_count = 0;
    
    for (int i = left; i < right; ++i) {
      if (a[i] == left_maj) {
        left_count += 1;
      } else if (a[i] == right_maj) {
        right_count += 1;
      }
    }

    int count = right - left;
    if (left_count > count / 2) {
      return left_maj;
    }
    else if (right_count > count / 2) {
      return right_maj;
    }
    return -1;
  }
}

int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); ++i) {
    std::cin >> a[i];
  }
  std::cout << (get_majority_element(a, 0, a.size()) != -1) << '\n';
}
