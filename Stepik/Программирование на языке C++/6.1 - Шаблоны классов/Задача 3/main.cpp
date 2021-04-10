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

struct ICloneable {
	virtual ICloneable* clone() const = 0;
	virtual ~ICloneable() { }
};

// ������ ValueHolder � ������� ���������� T,
// ������ ��������� ���� �������� ���� data_
// ���� T.
//
// � ������� ValueHolder ������ ���� ���������
// ����������� �� ������ ��������� ���� T,
// ������� �������������� ���� data_.
//
// ������ ValueHolder ������ �������������
// ��������� ICloneable, � ���������� ���������
// �� ����� �������, ��������� � ����, �� ������
// clone.

template <typename T>
struct ValueHolder : public ICloneable {
	ValueHolder(const T& data) : data_(data)
	{}

	ValueHolder* clone() const {
		return new ValueHolder(*this);
	}

	T data_;
};

int main() {
  return 0;
}
