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

template <typename T>
struct ValueHolder : public ICloneable {
  ValueHolder(const T& data) : data_(data)
  {}

  ValueHolder* clone() const {
    return new ValueHolder(*this);
  }

  T data_;
};

// Ёто класс, который вам нужно реализовать
class Any {
public:
  // ¬ классе Any должен быть конструктор,
  // который можно вызвать без параметров,
  // чтобы работал следующий код:
  //    Any empty; // empty ничего не хранит
  Any() : m_value_holder(0)
  {}

  // ¬ классе Any должен быть шаблонный
  // конструктор от одного параметра, чтобы
  // можно было создавать объекты типа Any,
  // например, следующим образом:
  //    Any i(10); // i хранит значение 10
  template <typename T>
  Any(const T& value) : m_value_holder(new ValueHolder<T>(value))
  {}

  // Ќе забудьте про деструктор. ¬се выделенные
  // ресурсы нужно освободить.
  ~Any() {
    delete m_value_holder;
  }

  // ¬ классе Any также должен быть конструктор
  // копировани€ (вам поможет метод clone
  // интерфейса ICloneable)
  Any(const Any& any) : m_value_holder(0) {
		if (any.m_value_holder) {
			m_value_holder = any.m_value_holder->clone();
		}
	}

  // ¬ классе должен быть оператор присваивани€ и/или
  // шаблонный оператор присваивани€, чтобы работал
  // следующий код:
  //    Any copy(i); // copy хранит 10, как и i
  //    empty = copy; // empty хранит 10, как и copy
  //    empty = 0; // а теперь empty хранит 0
  Any& operator=(const Any& any) {
    if (this != &any) {
      delete m_value_holder;
			m_value_holder = 0;

			if (any.m_value_holder) {
				m_value_holder = any.m_value_holder->clone();
			}
    }
    return *this;
  }

  template <typename T>
  Any& operator=(const T& value) {
    delete m_value_holder;
		m_value_holder = 0;

    m_value_holder = new ValueHolder<T>(value);
    return *this;
  }

  // Ќу и наконец, мы хотим уметь получать хранимое
  // значение, дл€ этого определите в классе Any
  // шаблонный метод cast, который возвращает
  // указатель на хранимое значение, или нулевой
  // указатель в случае несоответстви€ типов или
  // если объект Any ничего не хранит:
  //    int *iptr = i.cast<int>(); // *iptr == 10
  //    char *cptr = i.cast<char>(); // cptr == 0,
  //        // потому что i хранит int, а не char
  //    Any empty2;
  //    int *p = empty2.cast<int>(); // p == 0
  // ѕри реализации используйте оператор dynamic_cast.
  //
  // ƒопустим у вас есть два наследника класса Base: Derived1 
  // и Derived2.  роме того у вас есть указать baseptr
  // типа Base*.  ак проверить указывает ли этот указатель на
  // самом деле на объект класса Derived1 или на объект 
  // класса Derived2? ƒл€ этого можно воспользоватьс€ dynamic_cast-ом:
  //
  // Derived1 *derived1ptr = dynamic_cast<Derived1*>(baseptr);
  //
  // ≈сли derived1ptr не равен 0, то  baseptr на самом деле
  // указывал на объект класса Derived1, если же derivedptr равен 0,
  // то baseptr на самом деле указывал на объкт какого-то другого 
  // класса (например, Derived2). Ёто можно проверить так:
  //
  // Derived2 *derived2ptr = dynamic_cast<Derived2*>(baseptr);
  //
  // dynamic_cast работает только, если в классе есть хот€ бы
  // один виртуальный метод, и в шаблоне ValueHolder такой
  // как раз имеетс€.
  template <typename T>
  T* cast() {
    if (m_value_holder) {
      ValueHolder<T>* value_holder = dynamic_cast<ValueHolder<T>*>(m_value_holder);
      if (value_holder) {
        return &value_holder->data_;
      }
      return 0;
    }
    return 0;
  }

private:
  ICloneable* m_value_holder;
};

int main() {
  Any empty;
  Any i(10);

  Any copy(i);
  empty = copy;
  empty = 0;
  
  int* iptr = i.cast<int>(); // *iptr == 10
  char *cptr = i.cast<char>(); // cptr == 0, потому что i хранит int, а не char
  Any empty2;
  int *p = empty2.cast<int>(); // p == 0

  return 0;
}
