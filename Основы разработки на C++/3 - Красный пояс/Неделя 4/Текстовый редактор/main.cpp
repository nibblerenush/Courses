#include <list>
#include <string>
#include "test_runner.h"

using namespace std;

class Editor {
public:
  Editor();
  void Left();
  void Right();
  void Insert(char token);
  void Cut(size_t tokens = 1);
  void Copy(size_t tokens = 1);
  void Paste();
  string GetText() const;

private:
  std::list<char> m_letters;
  std::list<char>::iterator m_cur_pos;
  std::list<char> m_buffer;
};

Editor::Editor(): m_letters(), m_cur_pos(m_letters.end()), m_buffer()
{}

void Editor::Left()
{
  if (m_cur_pos != m_letters.begin())
  {
    m_cur_pos = std::prev(m_cur_pos);
  }
}

void Editor::Right()
{
  if (m_cur_pos != m_letters.end())
  {
    m_cur_pos = std::next(m_cur_pos);
  }
}

void Editor::Insert(char token)
{
  m_letters.insert(m_cur_pos, token);
}

void Editor::Cut(std::size_t tokens)
{
  m_buffer.clear();
  auto cut_end = std::distance(m_cur_pos, m_letters.end()) > tokens ? std::next(m_cur_pos, tokens) : m_letters.end();
  std::copy(m_cur_pos, cut_end, std::back_inserter(m_buffer));
  m_cur_pos = m_letters.erase(m_cur_pos, cut_end);
}

void Editor::Copy(std::size_t tokens)
{
  m_buffer.clear();
  auto copy_end = std::distance(m_cur_pos, m_letters.end()) > tokens ? std::next(m_cur_pos, tokens) : m_letters.end();
  std::copy(m_cur_pos, copy_end, std::back_inserter(m_buffer));
}

void Editor::Paste()
{
  m_letters.insert(m_cur_pos, m_buffer.begin(), m_buffer.end());
}

std::string Editor::GetText() const
{
  return std::string(m_letters.begin(), m_letters.end());
}

void TypeText(Editor& editor, const string& text) {
  for (char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for (size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for (size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);

    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;

    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for (char c : text) {
    editor.Insert(c);
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}
