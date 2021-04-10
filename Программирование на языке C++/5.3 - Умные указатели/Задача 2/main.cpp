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

struct SharedPtr
{
  // реализуйте следующие методы
  //
  explicit SharedPtr(Expression* ptr = 0) : counter_(new int (1)), ptr_(ptr)
  {}
  
  ~SharedPtr() {
    *counter_ -= 1;
    if (*counter_ == 0) {
      delete counter_;
      delete ptr_;
    }
  }
  
  SharedPtr(const SharedPtr& sharedPtr) : counter_(sharedPtr.counter_), ptr_(sharedPtr.ptr_) {
    *counter_ += 1;
  }
  
  SharedPtr& operator=(const SharedPtr& sharedPtr) {
    if (this != &sharedPtr) {
      *counter_ -= 1;
      if (*counter_ == 0) {
        delete counter_;
        delete ptr_;
      }
      
      counter_ = sharedPtr.counter_;
      ptr_ = sharedPtr.ptr_;
      *counter_ += 1;
    }

    return *this;
  }
  
  Expression* get() const {
    return ptr_;
  }
  
  void reset(Expression* ptr = 0) {
    *counter_ -= 1;
    if (*counter_ == 0) {
      delete counter_;
      delete ptr_;
    }

    counter_ = new int(1);
    ptr_ = ptr;
  }
  
  Expression& operator*() const {
    return *ptr_;
  }
  
  Expression* operator->() const {
    return ptr_;
  }

private:
  int* counter_;
  Expression* ptr_;
};

int main() {
  return 0;
}
