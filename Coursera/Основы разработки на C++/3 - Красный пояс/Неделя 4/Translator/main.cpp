#include "test_runner.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class Translator {
public:
  void Add(string_view source, string_view target);
  string_view TranslateForward(string_view source) const;
  string_view TranslateBackward(string_view target) const;

private:
  std::map<std::string, std::string> m_src_trg;
  std::map<std::string_view, std::string_view> m_trg_src;
};

void Translator::Add(std::string_view src, std::string_view trg)
{
  std::string local_src(src);

  {
    auto iter = m_src_trg.find(local_src);
    if (iter != m_src_trg.end())
    {
      m_trg_src.erase(iter->second);
    }
  }

  {
    auto iter = m_trg_src.find(trg);
    if (iter != m_trg_src.end())
    {
      m_src_trg.erase(std::string(iter->second));
      m_trg_src.erase(iter->first);
    }
  }

  m_src_trg[local_src] = trg;
  auto iter = m_src_trg.find(local_src);
  m_trg_src[iter->second] = iter->first;
}

std::string_view Translator::TranslateForward(std::string_view src) const
{
  std::string local_src(src);
  return m_src_trg.count(local_src) ? m_src_trg.at(local_src) : std::string_view();
}

std::string_view Translator::TranslateBackward(std::string_view trg) const
{
  return m_trg_src.count(trg) ? m_trg_src.at(trg) : std::string_view();
}

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

void TestHard() {
  Translator translator;
  translator.Add(string("okno1"), string("window1"));

  ASSERT_EQUAL(translator.TranslateForward("okno1"), "window1");
  ASSERT_EQUAL(translator.TranslateBackward("window1"), "okno1");

  translator.Add(string("okno1"), string("window2"));
  ASSERT_EQUAL(translator.TranslateForward("okno1"), "window2");
  ASSERT_EQUAL(translator.TranslateBackward("window2"), "okno1");

  translator.Add(string("okno2"), string("window1"));
  ASSERT_EQUAL(translator.TranslateForward("okno2"), "window1");
  ASSERT_EQUAL(translator.TranslateBackward("window1"), "okno2");

  translator.Add(string("okno1"), string("window1"));
  ASSERT_EQUAL(translator.TranslateForward("okno1"), "window1");
  ASSERT_EQUAL(translator.TranslateBackward("window1"), "okno1");

  translator.Add(string("okno2"), string("window2"));
  ASSERT_EQUAL(translator.TranslateForward("okno2"), "window2");
  ASSERT_EQUAL(translator.TranslateBackward("window2"), "okno2");

  ASSERT_EQUAL(translator.TranslateForward("ss"), "");
  ASSERT_EQUAL(translator.TranslateBackward("ss"), "");

  translator.Add(string("ss"), string("ss"));
  ASSERT_EQUAL(translator.TranslateForward("ss"), "ss");
  ASSERT_EQUAL(translator.TranslateBackward("ss"), "ss");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  RUN_TEST(tr, TestHard);
  return 0;
}
