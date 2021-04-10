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

struct Number;
struct BinaryOperation;

struct Visitor {
  virtual void visitNumber(Number const* number) = 0;
  virtual void visitBinaryOperation(BinaryOperation const* operation) = 0;
  virtual ~Visitor() { }
};

struct Expression
{
  virtual double evaluate() const = 0;
  virtual void visit(Visitor* vistitor) const = 0;
  virtual ~Expression() { }
};

struct Number : Expression
{
  Number(double value) : value(value) 
  {}
  
  double evaluate() const {
    return value;
  }

  double get_value() const { return value; }

  void visit(Visitor* visitor) const { visitor->visitNumber(this); }

private:
  double value;
};

struct BinaryOperation : Expression
{
  BinaryOperation(Expression const* left, char op, Expression const* right) :
    left(left), right(right), op(op)
  {}

  ~BinaryOperation() {
    delete left;
    delete right;
  }

  double evaluate() const {
    switch (op) {
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

  Expression const* get_left() const { return left; }
  Expression const* get_right() const { return right; }
  char get_op() const { return op; }

  void visit(Visitor* visitor) const { visitor->visitBinaryOperation(this); }

private:
  Expression const* left;
  Expression const* right;
  char op;
};

/* Этот класс вам нужно реализовать */
struct PrintVisitor : Visitor {
  void visitNumber(Number const* number) {
    cout << '(' << number->get_value() << ')';
  }

  void visitBinaryOperation(BinaryOperation const* bop) {
    cout << '(';
    bop->get_left()->visit(this);
    cout << bop->get_op();
    bop->get_right()->visit(this);
    cout << ')';
  }
};

int main() {
  // сначала создаём объекты для подвыражения 4.5 * 5
  Expression* sube = new BinaryOperation(new Number(4.5), '*', new Number(5));
  // потом используем его в выражении для +
  Expression* expr = new BinaryOperation(new Number(3), '+', sube);
  
  PrintVisitor visitor;
  expr->visit(&visitor);
  
  return 0;
}
