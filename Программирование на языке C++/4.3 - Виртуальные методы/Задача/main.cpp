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
    ����� op ��� ���� �� 4 ��������: '+', '-', '*' ��� '/', ��������������� ���������,
    ������� ��� ����� �����������.
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

int main() {
  // ������� ������ ������� ��� ������������ 4.5 * 5
  Expression* sube = new BinaryOperation(new Number(4.5), '*', new Number(5));
  // ����� ���������� ��� � ��������� ��� +
  Expression* expr = new BinaryOperation(new Number(3), '+', sube);

  // ��������� � ������� ���������: 25.5
  std::cout << expr->evaluate() << std::endl;

  // ��� ������������� *���* ���������� �������
  // (��������, sube ����� ������ ��������� expr, ������� ��� ������� �� �����)
  delete expr;

  return 0;
}
