#include <iostream>

using namespace std;

void func() {
  int a = 0;
  cin >> a;
  
  if (a == 0) {
    return;
  }
  else {
    func();
  }
  
  cout << a << ' ';
}

int main() {
  func();
  return 0;
}
