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

struct Expression {
  virtual double evaluate() const = 0;
  virtual ~Expression() {};
};

struct Number : Expression {
  Number(double value)
    : value(value)
  {}

  double evaluate() const {
    return value;
  }

private:
  double value;
};

struct BinaryOperation : Expression {
  /*
    Здесь op это один из 4 символов: '+', '-', '*' или '/', соответствующих операциям,
    которые вам нужно реализовать.
   */
  BinaryOperation(Expression const* left, char op, Expression const* right)
    : left(left), op(op), right(right)
  { }

  double evaluate() const {
    switch (op)
    {
    case '+':
      return left->evaluate() + right->evaluate();
    case '-':
      return left->evaluate() - right->evaluate();
    case '*':
      return left->evaluate() * right->evaluate();
    case '/':
      return left->evaluate() / right->evaluate();
    }
  }

  ~BinaryOperation() {
    delete left;
    delete right;
  }

private:
  Expression const* left;
  Expression const* right;
  char op;
};

bool check_equals_(Expression const* left, Expression const* right) {
  size_t ptr1 = *(size_t*)left;
  size_t ptr2 = *(size_t*)right;
  return ptr1 == ptr2;
}

int main() {
  Expression* left = new Number(0);
  Expression* right = new Number(1);
  //Expression* right = new BinaryOperation(nullptr, 0, nullptr);
  check_equals_(left, right);
  return 0;
}
