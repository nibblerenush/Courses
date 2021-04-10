#include "node.h"

bool EmptyNode::Evaluate(const Date&, const std::string&) const
{
  return true;
}

DateComparisonNode::DateComparisonNode(Comparison cmp, const Date& date) : m_cmp(cmp), m_date(date)
{}

bool DateComparisonNode::Evaluate(const Date& date, const std::string&) const
{
	switch (m_cmp)
	{
	case Comparison::Less:
		return date < m_date;
	case Comparison::LessOrEqual:
		return date <= m_date;
	case Comparison::Greater:
		return date > m_date;
	case Comparison::GreaterOrEqual:
		return date >= m_date;
	case Comparison::Equal:
		return date == m_date;
	case Comparison::NotEqual:
		return date != m_date;
	}
}

EventComparisonNode::EventComparisonNode(Comparison cmp, const std::string& event) : m_cmp(cmp), m_event(event)
{}

bool EventComparisonNode::Evaluate(const Date&, const std::string& event) const
{
	switch (m_cmp)
	{
	case Comparison::Less:
		return event < m_event;
	case Comparison::LessOrEqual:
		return event <= m_event;
	case Comparison::Greater:
		return event > m_event;
	case Comparison::GreaterOrEqual:
		return event >= m_event;
	case Comparison::Equal:
		return event == m_event;
	case Comparison::NotEqual:
		return event != m_event;
	}
}

LogicalOperationNode::LogicalOperationNode(LogicalOperation op, std::shared_ptr<Node> left, std::shared_ptr<Node> right):
	m_op(op), m_left(left), m_right(right)
{}


bool LogicalOperationNode::Evaluate(const Date& date, const std::string& event) const
{
	switch (m_op)
	{
	case LogicalOperation::Or:
		return m_left->Evaluate(date, event) || m_right->Evaluate(date, event);
	case LogicalOperation::And:
		return m_left->Evaluate(date, event) && m_right->Evaluate(date, event);
	}
}
