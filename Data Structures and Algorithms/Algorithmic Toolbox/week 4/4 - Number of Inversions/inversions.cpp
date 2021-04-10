#include <iostream>
#include <vector>

using std::vector;

long long merge(vector<int>& a, vector<int>& b, size_t left, size_t ave, size_t right) {
  size_t i = left;
  size_t j = ave;
  size_t k = left;
  long long result = 0;
  
  while (i < ave && j < right) {
    if (a[i] <= a[j]) {
      b[k++] = a[i++];
    }
    else {
      b[k++] = a[j++];
      result += ((long long)ave - left) - ((long long)i - left);
    }
  }

  while (i < ave) {
    b[k++] = a[i++];
  }

  while (j < right) {
    b[k++] = a[j++];
  }
  
  return result;
}

long long get_number_of_inversions(vector<int> &a, vector<int> &b, size_t left, size_t right) {
  long long number_of_inversions = 0;
  if (right <= left + 1) return number_of_inversions;
  size_t ave = left + (right - left) / 2;
  number_of_inversions += get_number_of_inversions(a, b, left, ave);
  number_of_inversions += get_number_of_inversions(a, b, ave, right);
  
  number_of_inversions += merge(a, b, left, ave, right);
  for (size_t i = left; i < right; ++i) {
    a[i] = b[i];
  }
  
  return number_of_inversions;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); i++) {
    std::cin >> a[i];
  }
  vector<int> b(a.size());
  std::cout << get_number_of_inversions(a, b, 0, a.size()) << '\n';

  return 0;
}
