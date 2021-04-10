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

template<class T>
struct Array
{
  // все объ€вленные ниже методы уже реализованы
  explicit Array(size_t size = 0);
  Array(Array const& a);
  Array& operator=(Array const& a);
  ~Array();

  size_t size() const;
  T& operator[](size_t i);
  T const& operator[](size_t i) const;

  // реализуйте перемещающий конструктор
  Array(Array&& arr) : size_(0), data_(0) {
    std::swap(size_, arr.size_);
    std::swap(data_, arr.data_);
  }

  // реализуйте перемещающий оператор присваивани€
  Array& operator=(Array&& arr) {
    std::swap(size_, arr.size_);
    std::swap(data_, arr.data_);

    return *this;
  }

private:
  size_t  size_ = 0;
  T* data_ = 0;
};

int main() {
  return 0;
}
