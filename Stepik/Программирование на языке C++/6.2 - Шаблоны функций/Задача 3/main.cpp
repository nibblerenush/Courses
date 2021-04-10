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
  explicit Array() : m_data(0), m_size(0)
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

template <typename T>
void flatten(const T& obj, std::ostream& out) {
  out << ' ' << obj << ' ';
}

template <typename T>
void flatten(const Array<T>& arr, std::ostream& out) {
  for (size_t i = 0; i < arr.size(); ++i) {
    flatten(arr[i], out);
  }
}

int main() {
  Array<int> ints(2, 0);
  ints[0] = 10;
  ints[1] = 20;
  flatten(ints, std::cout); // выводит на экран строку "10 20"

  cout << endl;

  Array< Array<int> > array_of_ints(2, ints);
  flatten(array_of_ints, std::cout); // выводит на экран строку "10 20 10 20"

  cout << endl;

  Array<double> doubles(10, 0.0);
  flatten(doubles, std::cout); // работать должно не только для типа int

  return 0;
}
