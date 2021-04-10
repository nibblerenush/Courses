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

// ��� �����, ������� ��� ����� �����������
class Any {
public:
  // � ������ Any ������ ���� �����������,
  // ������� ����� ������� ��� ����������,
  // ����� ������� ��������� ���:
  //    Any empty; // empty ������ �� ������
  Any() : m_value_holder(0)
  {}

  // � ������ Any ������ ���� ���������
  // ����������� �� ������ ���������, �����
  // ����� ���� ��������� ������� ���� Any,
  // ��������, ��������� �������:
  //    Any i(10); // i ������ �������� 10
  template <typename T>
  Any(const T& value) : m_value_holder(new ValueHolder<T>(value))
  {}

  // �� �������� ��� ����������. ��� ����������
  // ������� ����� ����������.
  ~Any() {
    delete m_value_holder;
  }

  // � ������ Any ����� ������ ���� �����������
  // ����������� (��� ������� ����� clone
  // ���������� ICloneable)
  Any(const Any& any) : m_value_holder(0) {
		if (any.m_value_holder) {
			m_value_holder = any.m_value_holder->clone();
		}
	}

  // � ������ ������ ���� �������� ������������ �/���
  // ��������� �������� ������������, ����� �������
  // ��������� ���:
  //    Any copy(i); // copy ������ 10, ��� � i
  //    empty = copy; // empty ������ 10, ��� � copy
  //    empty = 0; // � ������ empty ������ 0
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

  // �� � �������, �� ����� ����� �������� ��������
  // ��������, ��� ����� ���������� � ������ Any
  // ��������� ����� cast, ������� ����������
  // ��������� �� �������� ��������, ��� �������
  // ��������� � ������ �������������� ����� ���
  // ���� ������ Any ������ �� ������:
  //    int *iptr = i.cast<int>(); // *iptr == 10
  //    char *cptr = i.cast<char>(); // cptr == 0,
  //        // ������ ��� i ������ int, � �� char
  //    Any empty2;
  //    int *p = empty2.cast<int>(); // p == 0
  // ��� ���������� ����������� �������� dynamic_cast.
  //
  // �������� � ��� ���� ��� ���������� ������ Base: Derived1 
  // � Derived2. ����� ���� � ��� ���� ������� baseptr
  // ���� Base*. ��� ��������� ��������� �� ���� ��������� ��
  // ����� ���� �� ������ ������ Derived1 ��� �� ������ 
  // ������ Derived2? ��� ����� ����� ��������������� dynamic_cast-��:
  //
  // Derived1 *derived1ptr = dynamic_cast<Derived1*>(baseptr);
  //
  // ���� derived1ptr �� ����� 0, ��  baseptr �� ����� ����
  // �������� �� ������ ������ Derived1, ���� �� derivedptr ����� 0,
  // �� baseptr �� ����� ���� �������� �� ����� ������-�� ������� 
  // ������ (��������, Derived2). ��� ����� ��������� ���:
  //
  // Derived2 *derived2ptr = dynamic_cast<Derived2*>(baseptr);
  //
  // dynamic_cast �������� ������, ���� � ������ ���� ���� ��
  // ���� ����������� �����, � � ������� ValueHolder �����
  // ��� ��� �������.
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
  char *cptr = i.cast<char>(); // cptr == 0, ������ ��� i ������ int, � �� char
  Any empty2;
  int *p = empty2.cast<int>(); // p == 0

  return 0;
}
