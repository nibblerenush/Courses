#include <iostream>

using namespace std;

void reverse(int a[], int start, int end) {
  while (start < end) {
    int temp = a[start];
    a[start] = a[end];
    a[end] = temp;
    start += 1;
    end -= 1;
  }
}

void rotate(int a[], unsigned size, int shift) {
  shift %= size;

  reverse(a, 0, size - 1);
  reverse(a, 0, size - shift - 1);
  reverse(a, size - shift, size - 1);
}

int main() {
  int a[] = { 1,2,3,4,5 };
  rotate(a, 5, 0);

  for (int i = 0; i < 5; ++i) {
    cout << a[i] << ' ';
  }

  return 0;
}
