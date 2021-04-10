#include <iostream>

using namespace std;

void strcat_(char* to, const char* from) {
  while (*to != '\0') {
    to += 1;
  }

  while (*from != '\0') {
    *to = *from;
    to += 1;
    from += 1;
  }

  *to = '\0';
}

int main() {
  char to[11] = "11111";
  strcat_(to, "22222");
  cout << to;
  return 0;
}
