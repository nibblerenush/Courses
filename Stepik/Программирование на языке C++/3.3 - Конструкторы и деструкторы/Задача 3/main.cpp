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
	String(const char* str = "") {
		this->size = strlen(str);
		this->str = new char[size + 1];

		copy(str, str + this->size, this->str);
		this->str[size] = '\0';
	}


	String(size_t n, char c) {
		size = n;
		str = new char[size + 1];
		fill(str, str + size, c);
		str[size] = '\0';
	}

	~String() {
		delete[] str;
	}


	/* Реализуйте этот метод. */
	void append(String& other) {
		char* new_str = new char[size + other.size + 1];
		copy(str, str + size, new_str);
		copy(other.str, other.str + other.size, new_str + size);
		delete[] str;
		str = new_str;
		size = size + other.size;
		str[size] = '\0';
	}
	
	size_t size;
	char* str;
};

int main() {
	String s1("Hello,");
	String s2(" world!");

	s1.append(s2); // теперь s1 хранит "Hello, world!"
	cout << s1.str << endl;

	String s("Hello");
	s.append(s); // теперь s хранит "HelloHello"
	cout << s.str << endl;

  return 0;
}
