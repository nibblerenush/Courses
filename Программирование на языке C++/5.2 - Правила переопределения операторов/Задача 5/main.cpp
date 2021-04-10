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
	struct Helper {
		Helper(const char* str, int begin) : str_(str), begin_(begin)
		{}

		String operator[](int end) const {
			int size = end - begin_;
			if (size == 0) {
				return String();
			}

			char* new_str = new char[size + 1];
			copy(str_ + begin_, str_ + end, new_str);
			new_str[size] = '\0';
			String result(new_str);
			delete[] new_str;
			return result;
		}

		const char* str_;
		int begin_;
	};

	Helper operator[](int begin) const {
		Helper helper(str, begin);
		return helper;
	}

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

	String(const String& other) {
		size = other.size;
		str = new char[size + 1];

		copy(other.str, other.str + size, str);
		str[size] = '\0';
	}

	~String() {
		delete[] str;
	}

	String& operator=(const String& other) {
		if (this != &other) {
			delete[] str;

			size = other.size;
			str = new char[size + 1];

			copy(other.str, other.str + size, str);
			str[size] = '\0';
		}

		return *this;
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
	String const hello("hello");
	String const hell = hello[0][4]; // теперь в hell хранится подстрока "hell"
	String const ell = hello[1][4]; // теперь в ell хранится подстрока "ell"

  return 0;
}
