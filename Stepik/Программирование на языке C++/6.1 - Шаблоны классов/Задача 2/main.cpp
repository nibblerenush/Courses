#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <new>
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
  explicit Array(): m_data(0), m_size(0)
  {}

  /*
  * конструктор класса, который создает
    Array размера size, заполненный значениями
    value типа T. Считайте что у типа T есть
    конструктор, который можно вызвать без
    без параметров, либо он ему не нужен.
  */
  explicit Array(size_t size = 0, const T& value = T()) : m_data(0), m_size(0) {
    m_size = size;
    char* memory = new char[m_size * sizeof(T)];
    m_data = (T*)memory;
    for (size_t i = 0; i < m_size; ++i) {
      new (memory + i * sizeof(T)) T(value);
    }
  }

  /*
  * конструктор копирования, который создает
    копию параметра. Считайте, что для типа
    T определен оператор присваивания.
  */
  Array(const Array& arr) {
    m_size = arr.m_size;
    char* memory = new char[m_size * sizeof(T)];
    m_data = (T*)memory;
    for (size_t i = 0; i < m_size; ++i) {
      new (memory + i * sizeof(T)) T(arr[i]);
    }
  }

  // деструктор, если он вам необходим.
  ~Array() {
    T* data = m_data;
    for (size_t i = 0; i < m_size; ++i) {
      data->~T();
      data += 1;
    }

    char* memory = (char*)m_data;
    delete[] memory;
  }

  // оператор присваивания.
  Array& operator=(const Array& arr) {
    if (this != &arr) {
      T* data = m_data;
      for (size_t i = 0; i < m_size; ++i) {
        data->~T();
        data += 1;
      }
      
      char* memory = (char*)m_data;
      delete[] memory;
      
      m_size = arr.m_size;
      memory = new char[m_size * sizeof(T)];
      m_data = (T*)memory;
      for (size_t i = 0; i < m_size; ++i) {
        new (memory + i * sizeof(T)) T(arr[i]);
      }
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

struct Test {
  Test() = delete;
  
  Test(const Test& test) {
    cout << ++count << " Test\n";
  }

  Test(int)
  {}

  ~Test() {
    cout << count-- << "~Test\n";
  }

  static int count;
};

int Test::count = 0;

int main() {
  Array<int> arr1(10, 5);
  cout << arr1[0] << '\n';
  for (size_t i = 0; i < arr1.size(); ++i) {
    cout << arr1[i];
  }

  Test test(1);
  Array<Test> arr2(10, test);
  return 0;
}
