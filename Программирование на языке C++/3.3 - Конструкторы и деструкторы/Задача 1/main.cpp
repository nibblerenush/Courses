#include <algorithm>
#include <cmath>
#include <cstring>
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

struct String {
	/* Реализуйте этот конструктор */
	String(const char* str = "") {
		this->size = strlen(str);
		this->str = new char[size + 1];

		copy(str, str + this->size, this->str);
		this->str[size] = '\0';
	}
	
	size_t size;
	char* str;
};

int main() {
  return 0;
}
