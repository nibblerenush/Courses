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

// Ўаблон ValueHolder с типовым параметром T,
// должен содержать одно открытое поле data_
// типа T.
//
// ¬ шаблоне ValueHolder должен быть определен
// конструктор от одного параметра типа T,
// который инициализирует поле data_.
//
// Ўаблон ValueHolder должен реализовывать
// интерфейс ICloneable, и возвращать указатель
// на копию объекта, созданную в куче, из метода
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
