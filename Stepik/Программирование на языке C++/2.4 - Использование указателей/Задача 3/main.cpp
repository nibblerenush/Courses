#include <iostream>

using namespace std;

int strstr_(const char* text, const char* pattern) {
  if (*pattern == '\0') {
    return 0;
  }
  
  int index = 0;
  bool is_found = false;

  for (const char* t = text; *t != '\0'; ++t) {
    const char* temp = t;
    is_found = true;

    for (const char* p = pattern; *p != '\0'; ++p) {
      if (*temp == '\0' || *p != *temp) {
        is_found = false;
        break;
      }
      temp += 1;
    }

    if (is_found) {
      break;
    }

    index += 1;
  }

  return is_found ? index : -1;
}

int main() {
  cout << strstr_("abba", "") << endl;
  return 0;
}
