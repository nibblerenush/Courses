#include "test_runner.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>

void PrintJsonString(std::ostream& out, std::string_view str) {
  out << '\"';
  for (const auto& letter : str) {
    if (letter == '\"' || letter == '\\') {
      out << '\\';
    }
    out << letter;
  }
  out << '\"';
}

class NullContext
{};

template <typename Parent>
class InArrayContext;

template <typename Parent>
class InObjectKeyContext;

template <typename Parent>
class InObjectValueContext;

template <typename Parent>
class InArrayContext
{
public:
  InArrayContext(std::ostream& out, Parent& parent) : m_out(out), m_parent(parent) {
    out << '[';
  }

  InArrayContext<Parent>& Number(std::int64_t number) {
    IsFirst();
    m_out << number;
    return *this;
  }

  InArrayContext<Parent>& String(std::string_view str) {
    IsFirst();
    PrintJsonString(m_out, str);
    return *this;
  }

  InArrayContext<Parent>& Boolean(bool bool_value) {
    IsFirst();
    m_out << std::boolalpha << bool_value;
    return *this;
  }

  InArrayContext<Parent>& Null() {
    IsFirst();
    m_out << "null";
    return *this;
  }

  InArrayContext<InArrayContext<Parent>> BeginArray() {
    IsFirst();
    return InArrayContext<InArrayContext<Parent>>(m_out, *this);
  }

  Parent& EndArray() {
    m_ended = true;
    m_out << ']';
    return m_parent;
  }

  InObjectKeyContext<InArrayContext<Parent>> BeginObject() {
    IsFirst();
    return InObjectKeyContext<InArrayContext<Parent>>(m_out, *this);
  }

  ~InArrayContext() {
    if (!m_ended) {
      m_out << ']';
    }
  }

private:
  void IsFirst() {
    if (m_first) {
      m_first = false;
    }
    else {
      m_out << ',';
    }
  }
  
private:
  std::ostream& m_out;
  Parent& m_parent;
  bool m_ended = false;
  bool m_first = true;
};

template <typename Parent>
class InObjectKeyContext
{
public:
  InObjectKeyContext(std::ostream& out, Parent& parent) : m_out(out), m_parent(parent) {
    m_out << '{';
  }

  InObjectValueContext<InObjectKeyContext<Parent>> Key(std::string_view key) {
    IsFirst();
    PrintJsonString(m_out, key);
    m_out << ':';
    return InObjectValueContext<InObjectKeyContext<Parent>>(m_out, *this);
  }

  Parent& EndObject() {
    m_ended = true;
    m_out << '}';
    return m_parent;
  }

  ~InObjectKeyContext() {
    if (!m_ended) {
      m_out << '}';
    }
  }

private:
  void IsFirst() {
    if (m_first) {
      m_first = false;
    }
    else {
      m_out << ',';
    }
  }

private:
  std::ostream& m_out;
  Parent& m_parent;
  bool m_ended = false;
  bool m_first = true;
};

template <typename Parent>
class InObjectValueContext
{
public:
  InObjectValueContext(std::ostream& out, Parent& parent) : m_out(out), m_parent(parent)
  {}

  Parent& Number(std::int64_t number) {
    m_ended = true;
    m_out << number;
    return m_parent;
  }

  Parent& String(std::string_view str) {
    m_ended = true;
    PrintJsonString(m_out, str);
    return m_parent;
  }

  Parent& Boolean(bool bool_value) {
    m_ended = true;
    m_out << std::boolalpha << bool_value;
    return m_parent;
  }

  Parent& Null() {
    m_ended = true;
    m_out << "null";
    return m_parent;
  }

  InArrayContext<Parent> BeginArray() {
    m_ended = true;
    return InArrayContext<Parent>(m_out, m_parent);
  }

  InObjectKeyContext<Parent> BeginObject() {
    m_ended = true;
    return InObjectKeyContext<Parent>(m_out, m_parent);
  }

  ~InObjectValueContext() {
    if (!m_ended) {
      m_out << "null";
    }
  }

private:
  std::ostream& m_out;
  Parent& m_parent;
  bool m_ended = false;
};

using ArrayContext = InArrayContext<NullContext>;

ArrayContext PrintJsonArray(std::ostream& out) {
  NullContext null_context;
  return ArrayContext(out, null_context);
}

using ObjectContext = InObjectKeyContext<NullContext>;

ObjectContext PrintJsonObject(std::ostream& out) {
  NullContext null_context;
  return ObjectContext(out, null_context);
}

void TestArray() {
  std::ostringstream output;

  {
    auto json = PrintJsonArray(output);
    json
      .Number(5)
      .Number(6)
      .BeginArray()
        .Number(7)
      .EndArray()
      .Number(8)
      .String("bingo!");
  }

  ASSERT_EQUAL(output.str(), R"([5,6,[7],8,"bingo!"])");
}

void TestObject() {
  std::ostringstream output;

  {
    auto json = PrintJsonObject(output);
    json
      .Key("id1").Number(1234)
      .Key("id2").Boolean(false)
      .Key("").Null()
      .Key("\"").String("\\");
  }

  ASSERT_EQUAL(output.str(), R"({"id1":1234,"id2":false,"":null,"\"":"\\"})");
}

void TestAutoClose() {
  std::ostringstream output;

  {
    auto json = PrintJsonArray(output);
    json.BeginArray().BeginObject();
  }

  ASSERT_EQUAL(output.str(), R"([[{}]])");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestArray);
  RUN_TEST(tr, TestObject);
  RUN_TEST(tr, TestAutoClose);
  
  return 0;
}
