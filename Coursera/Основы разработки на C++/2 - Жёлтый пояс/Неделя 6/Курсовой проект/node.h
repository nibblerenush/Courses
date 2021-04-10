#pragma once

#include "date.h"
#include <memory>
#include <string>

enum class Comparison
{
  Less,
  LessOrEqual,
  Greater,
  GreaterOrEqual,
  Equal,
  NotEqual
};

enum class LogicalOperation
{
  Or,
  And
};

class Node
{
public:
  virtual bool Evaluate(const Date& date, const std::string& event) const = 0;
};

class EmptyNode : public Node
{
public:
  bool Evaluate(const Date& date, const std::string& event) const override;
};

class DateComparisonNode : public Node
{
public:
  DateComparisonNode(Comparison cmp, const Date& date);
  bool Evaluate(const Date& date, const std::string& event) const override;
private:
  const Comparison m_cmp;
  const Date m_date;
};

class EventComparisonNode : public Node
{
public:
  EventComparisonNode(Comparison cmp, const std::string& event);
  bool Evaluate(const Date& date, const std::string& event) const override;
private:
  const Comparison m_cmp;
  const std::string m_event;
};

class LogicalOperationNode : public Node
{
public:
  LogicalOperationNode(LogicalOperation op, std::shared_ptr<Node> left, std::shared_ptr<Node> right);
  bool Evaluate(const Date& date, const std::string& event) const override;
private:
  const LogicalOperation m_op;
  const std::shared_ptr<Node> m_left;
  const std::shared_ptr<Node> m_right;
};
