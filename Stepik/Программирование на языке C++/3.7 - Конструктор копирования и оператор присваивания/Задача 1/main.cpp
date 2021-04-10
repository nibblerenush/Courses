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

	/* Реализуйте конструктор копирования */
	String(const String& other) {
		size = other.size;
		str = new char[size + 1];

		copy(other.str, other.str + size, str);
		str[size] = '\0';
	}

	~String() {
		delete[] str;
	}

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
  return 0;
}
