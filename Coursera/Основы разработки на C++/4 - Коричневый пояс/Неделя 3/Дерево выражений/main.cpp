#include "Common.h"
#include "test_runner.h"

#include <sstream>

using namespace std;

class ValueExpr : public Expression
{
public:
  ValueExpr(int value) : m_value(value)
  {}

  int Evaluate() const override
  {
    return m_value;
  }

  string ToString() const override
  {
    return to_string(m_value);
  }

private:
  int m_value;
};

class OperExpr : public Expression
{
public:
  OperExpr(char oper, ExpressionPtr left, ExpressionPtr right) : m_oper(oper), m_left(move(left)), m_right(move(right))
  {}

  int Evaluate() const override
  {
    switch (m_oper)
    {
    case '+':
      return m_left->Evaluate() + m_right->Evaluate();
      break;
    case '*':
      return m_left->Evaluate() * m_right->Evaluate();
      break;
    default:
      return 0;
      break;
    }
  }

  string ToString() const override
  {
    ostringstream oss;
    oss << '(' << m_left->ToString() << ')' << m_oper << '(' << m_right->ToString() << ')';
    return oss.str();
  }

private:
  char m_oper;
  ExpressionPtr m_left;
  ExpressionPtr m_right;
};

ExpressionPtr Value(int value)
{
  return make_unique<ValueExpr>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right)
{
  return make_unique<OperExpr>('+', move(left), move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right)
{
  return make_unique<OperExpr>('*', move(left), move(right));
}

string Print(const Expression* e) {
  if (!e) {
    return "Null expression provided";
  }
  stringstream output;
  output << e->ToString() << " = " << e->Evaluate();
  return output.str();
}

void Test() {
  ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
  ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

  ExpressionPtr e2 = Sum(move(e1), Value(5));
  ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

  ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);
  return 0;
}
