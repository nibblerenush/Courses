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
   * конструктор класса, который создает
     Array размера size, заполненный значениями
     value типа T. Считайте что у типа T есть
     конструктор, который можно вызвать без
     без параметров, либо он ему не нужен.
   */
  explicit Array(size_t size = 0, const T& value = T()) {
    m_size = size;
    m_data = new T[m_size];
    fill(m_data, m_data + m_size, value);
  }

  /*
  * конструктор копирования, который создает
    копию параметра. Считайте, что для типа
    T определен оператор присваивания.
  */
  Array(const Array& arr) {
    m_size = arr.m_size;
    m_data = new T[m_size];
    copy(arr.m_data, arr.m_data + m_size, m_data);
  }
     
  // деструктор, если он вам необходим.
  ~Array() {
    delete[] m_data;
  }
     
  // оператор присваивания.
  Array& operator=(const Array& arr) {
    if (this != &arr) {
      delete[] m_data;
      m_size = arr.m_size;
      m_data = new T[m_size];
      copy(arr.m_data, arr.m_data + m_size, m_data);
    }

    return *this;
  }
  
  // возвращает размер массива(количество элементов).
  size_t size() const {
    return m_size;
  }
     
  // две версии оператора доступа по индексу
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
