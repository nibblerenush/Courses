#include <algorithm>
#include <cmath>
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
	String(size_t n, char c) {
		size = n;
		str = new char[size + 1];
		fill(str, str + size, c);
		str[size] = '\0';
	}
	
	/* и деструктор */
	~String() {
		delete[] str;
	}
	
	size_t size;
	char* str;
};

int main() {
  return 0;
}
