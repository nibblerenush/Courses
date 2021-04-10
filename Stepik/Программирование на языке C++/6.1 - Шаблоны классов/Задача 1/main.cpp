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

template <typename T>
class Array {
public:
   /*
   * ����������� ������, ������� �������
     Array ������� size, ����������� ����������
     value ���� T. �������� ��� � ���� T ����
     �����������, ������� ����� ������� ���
     ��� ����������, ���� �� ��� �� �����.
   */
  explicit Array(size_t size = 0, const T& value = T()) {
    m_size = size;
    m_data = new T[m_size];
    fill(m_data, m_data + m_size, value);
  }

  /*
  * ����������� �����������, ������� �������
    ����� ���������. ��������, ��� ��� ����
    T ��������� �������� ������������.
  */
  Array(const Array& arr) {
    m_size = arr.m_size;
    m_data = new T[m_size];
    copy(arr.m_data, arr.m_data + m_size, m_data);
  }
     
  // ����������, ���� �� ��� ���������.
  ~Array() {
    delete[] m_data;
  }
     
  // �������� ������������.
  Array& operator=(const Array& arr) {
    if (this != &arr) {
      delete[] m_data;
      m_size = arr.m_size;
      m_data = new T[m_size];
      copy(arr.m_data, arr.m_data + m_size, m_data);
    }

    return *this;
  }
  
  // ���������� ������ �������(���������� ���������).
  size_t size() const {
    return m_size;
  }
     
  // ��� ������ ��������� ������� �� �������
  T& operator[](size_t index) {
    return m_data[index];
  }
  
  const T& operator[](size_t index) const {
    return m_data[index];
  }

private:
  T* m_data;
  size_t m_size;
};

int main() {
  Array<int> arr(10, 5);
  for (size_t i = 0; i < arr.size(); ++i) {
    cout << arr[i];
  }
  return 0;
}
