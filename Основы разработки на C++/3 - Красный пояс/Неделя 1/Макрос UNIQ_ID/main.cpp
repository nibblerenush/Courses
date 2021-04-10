#include <string>
#include <vector>
using namespace std;

#define UNIQ_ID2(value) name ## value  
#define UNIQ_ID1(value) UNIQ_ID2(value)
#define UNIQ_ID UNIQ_ID1(__LINE__)

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = { "hello", "world" };
  vector<int> UNIQ_ID = { 1, 2, 3, 4 };
}
