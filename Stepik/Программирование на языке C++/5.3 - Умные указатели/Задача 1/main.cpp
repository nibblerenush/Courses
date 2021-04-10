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

struct Expression;
struct Number;
struct BinaryOperation;

struct ScopedPtr {
  // реализуйте следующие методы:
  //
  explicit ScopedPtr(Expression *ptr = 0) : ptr_(ptr)
  {}

  ~ScopedPtr() {
    delete ptr_;
  }
  
  Expression* get() const {
    return ptr_;
  }
  
  Expression* release() {
    Expression* temp = ptr_;
    ptr_ = 0;
    return temp;
  }
  
  void reset(Expression* ptr = 0) {
    delete ptr_;
    ptr_ = ptr;
  }
  
  Expression& operator*() const {
    return *ptr_;
  }
  
  Expression* operator->() const {
    return ptr_;
  }

private:
  // запрещаем копирование ScopedPtr
  ScopedPtr(const ScopedPtr&);
  ScopedPtr& operator=(const ScopedPtr&);

  Expression* ptr_;
};

int main() {
  return 0;
}
