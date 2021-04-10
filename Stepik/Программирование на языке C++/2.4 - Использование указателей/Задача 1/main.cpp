#include <iostream>

using namespace std;

unsigned strlen_(const char* str) {
  unsigned result = 0;
  for (const char* c = str; *c != '\0'; ++c) {
    result += 1;
  }
  return result;
}

int main() {
  cout << strlen_("") << endl;
  return 0;
}
