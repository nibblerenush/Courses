#include "lexer.h"

#include <algorithm>
#include <charconv>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

using namespace std;

namespace Parse {

bool operator == (const Token& lhs, const Token& rhs) {
  using namespace TokenType;

  if (lhs.index() != rhs.index()) {
    return false;
  }
  if (lhs.Is<Char>()) {
    return lhs.As<Char>().value == rhs.As<Char>().value;
  } else if (lhs.Is<Number>()) {
    return lhs.As<Number>().value == rhs.As<Number>().value;
  } else if (lhs.Is<String>()) {
    return lhs.As<String>().value == rhs.As<String>().value;
  } else if (lhs.Is<Id>()) {
    return lhs.As<Id>().value == rhs.As<Id>().value;
  } else {
    return true;
  }
}

std::ostream& operator << (std::ostream& os, const Token& rhs) {
  using namespace TokenType;

#define VALUED_OUTPUT(type) \
  if (auto p = rhs.TryAs<type>()) return os << #type << '{' << p->value << '}';

  VALUED_OUTPUT(Number);
  VALUED_OUTPUT(Id);
  VALUED_OUTPUT(String);
  VALUED_OUTPUT(Char);

#undef VALUED_OUTPUT

#define UNVALUED_OUTPUT(type) \
    if (rhs.Is<type>()) return os << #type;

  UNVALUED_OUTPUT(Class);
  UNVALUED_OUTPUT(Return);
  UNVALUED_OUTPUT(If);
  UNVALUED_OUTPUT(Else);
  UNVALUED_OUTPUT(Def);
  UNVALUED_OUTPUT(Newline);
  UNVALUED_OUTPUT(Print);
  UNVALUED_OUTPUT(Indent);
  UNVALUED_OUTPUT(Dedent);
  UNVALUED_OUTPUT(And);
  UNVALUED_OUTPUT(Or);
  UNVALUED_OUTPUT(Not);
  UNVALUED_OUTPUT(Eq);
  UNVALUED_OUTPUT(NotEq);
  UNVALUED_OUTPUT(LessOrEq);
  UNVALUED_OUTPUT(GreaterOrEq);
  UNVALUED_OUTPUT(None);
  UNVALUED_OUTPUT(True);
  UNVALUED_OUTPUT(False);
  UNVALUED_OUTPUT(Eof);

#undef UNVALUED_OUTPUT

  return os << "Unknown token :(";
}

Lexer::Lexer(std::istream& input):
  m_input(input),
  m_is_new_line(false),
  m_cur_line_stream(),
  m_cur_indent(0),
  m_cur_token()
  {
    NextToken();
  }

const Token& Lexer::CurrentToken() const {
  return m_cur_token;
}

Token Lexer::NextToken()
{
  if (!m_indent_storage.empty())
  {
    m_cur_token = m_indent_storage.back();
    m_indent_storage.pop_back();
    return m_cur_token;
  }

  char c;
  m_cur_line_stream.get(c);

  if (m_cur_line_stream.eof())
  {
    while (true)
    {
      // If need new line
      if (m_is_new_line)
      {
        m_is_new_line = false;
        m_cur_token = TokenType::Newline{};
        break;
      }

      std::string cur_line;
      std::getline(m_input, cur_line);

      // Input is ended
      if (m_input.eof() && cur_line.empty())
      {
        if (m_cur_indent > 0)
        {
          m_cur_token = TokenType::Dedent{};
          std::size_t storage_size = m_cur_indent / 2 - 1;
          m_indent_storage.assign(storage_size, TokenType::Dedent{});
          m_cur_indent = 0;
          break;
        }
        
        m_cur_token = TokenType::Eof{};
        break;
      }

      // Empty line
      if (cur_line.empty())
      {
        continue;
      }

      // Current line
      m_is_new_line = true;
      std::istringstream cur_line_stream{std::move(cur_line)};
      m_cur_line_stream = std::move(cur_line_stream);
      int prev_indent = m_cur_indent;
      m_cur_indent = 0;

      // Count spaces
      while (true)
      {
        m_cur_line_stream.get(c);

        if (c == ' ')
        {
          m_cur_indent++;
        }
        else
        {
          m_cur_line_stream.putback(c);
          break;
        }
      }

      // Count indent storage
      std::size_t storage_size = std::abs(m_cur_indent - prev_indent) / 2 - 1;

      // Check indent
      if (m_cur_indent > prev_indent)
      {
        m_cur_token = TokenType::Indent{};
        m_indent_storage.assign(storage_size, TokenType::Indent{});
      }
      else if (m_cur_indent < prev_indent)
      {
        m_cur_token = TokenType::Dedent{};
        m_indent_storage.assign(storage_size, TokenType::Dedent{});
      }
      else
      {
        m_cur_token = GetVisibleLexem();
      }

      break;
    }
  }
  else
  {
    m_cur_line_stream.putback(c);
    m_cur_token = GetVisibleLexem();
  }

  return m_cur_token;
}

Token Lexer::GetVisibleLexem()
{
  char c;
  m_cur_line_stream.get(c);
  m_cur_line_stream.putback(c);

  if (std::isalpha(c) || c == '_')
  {
    return GetIdOrKeyWord();
  }
  else if (std::isdigit(c))
  {
    return GetNumber();
  }
  else if (c == '\'' || c == '\"')
  {
    return GetString();
  }
  else if (c == '=' || c == '>' || c == '<' || c == '!')
  {
    return GetCompare();
  }
  else if (c == ' ')
  {
    SkipSpaces();
    return GetVisibleLexem();
  }
  else
  {
    m_cur_line_stream.get(c);
    return TokenType::Char{c};
  }
}

Token Lexer::GetIdOrKeyWord()
{
  static const std::unordered_map<std::string, Token> key_words = {
    {"class", TokenType::Class{}},
    {"return", TokenType::Return{}},
    {"if", TokenType::If{}},
    {"else", TokenType::Else{}},
    {"def", TokenType::Def{}},
    {"print", TokenType::Print{}},
    {"or", TokenType::Or{}},
    {"None", TokenType::None{}},
    {"and", TokenType::And{}},
    {"not", TokenType::Not{}},
    {"True", TokenType::True{}},
    {"False", TokenType::False{}}
  };

  std::string name;

  while (true)
  {
    char c;
    m_cur_line_stream.get(c);

    if (!m_cur_line_stream) {
      break;
    }

    if (std::isalnum(c) || c == '_') {
      name.push_back(c);
    }
    else {
      m_cur_line_stream.putback(c);
      break;
    }
  }

  if (key_words.count(name)) {
    return key_words.at(name);
  }
  else {
    return TokenType::Id{name};
  }
}

Token Lexer::GetNumber()
{
  std::string number;

  while (true)
  {
    char c;
    m_cur_line_stream.get(c);

    if (!m_cur_line_stream) {
      break;
    }

    if (std::isdigit(c)) {
      number.push_back(c);
    }
    else {
      m_cur_line_stream.putback(c);
      break;
    }
  }
  
  return TokenType::Number{std::stoi(number)};
}

Token Lexer::GetString()
{
  char c;
  m_cur_line_stream.get(c);
  
  std::string str;
  std::getline(m_cur_line_stream, str, c);
  
  return TokenType::String{str};
}

Token Lexer::GetCompare()
{
  static const std::unordered_map<std::string, Token> compare_op = {
    {"==", TokenType::Eq{}},
    {"<=", TokenType::LessOrEq{}},
    {">=", TokenType::GreaterOrEq{}},
    {"!=", TokenType::NotEq{}}
  };
  
  char compare;
  m_cur_line_stream.get(compare);
  
  char equal;
  m_cur_line_stream.get(equal);
  
  if (!m_cur_line_stream) {
    return TokenType::Char{compare};
  }
  
  if (equal == '=') {
    std::string op = {compare, equal};
    return compare_op.at(op);
  }
  else {
    m_cur_line_stream.putback(equal);
    return TokenType::Char{compare};
  }
}

void Lexer::SkipSpaces()
{
  while (true)
  {
    char c;
    m_cur_line_stream.get(c);
    
    if (!m_cur_line_stream) {
      break;
    }
    
    if (c != ' ') {
      m_cur_line_stream.putback(c);
      break;
    }
  }
}

} /* namespace Parse */
