#include "test_runner.h"

#include <vector>

using namespace std;

// ��������� Sentence<Token> ��� ������������� ���� Token
// ��������� vector<Token>.
// ��������� ����� � �������� ������������� ��������
// ������� ����� ������� �� ������������ ������ ��������,
// � ������ ����������� � vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

// ����� Token ����� ����� bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
  // �������� ���������� �������, �� ������� ������� ���� Token
  std::vector<Sentence<Token>> results;

  if (tokens.empty() || tokens.size() == 1)
  {
    results.push_back(std::move(tokens));
    return results;
  }
  
  Sentence<Token> result;
  result.push_back(std::move(tokens[0]));
  for (std::size_t i = 1; i < tokens.size(); ++i)
  {
    if (!tokens[i].IsEndSentencePunctuation() && tokens[i - 1].IsEndSentencePunctuation())
    {
      results.push_back(std::move(result));
    }
    result.push_back(std::move(tokens[i]));
  }
  if (!result.empty())
  {
    results.push_back(std::move(result));
  }
  return results;
}


struct TestToken {
  string data;
  bool is_end_sentence_punctuation = false;

  bool IsEndSentencePunctuation() const {
    return is_end_sentence_punctuation;
  }
  bool operator==(const TestToken& other) const {
    return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
  return stream << token.data;
}

// ���� �������� ����������� �������� ������ TestToken.
// ��� �������� ���������� ����������� � ������� SplitIntoSentences
// ���������� �������� ��������� ����.
void TestSplitting() {
  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!"} })),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!"}}
      })
    );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!", true} })),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
      })
    );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true} })),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
        {{"Without"}, {"copies"}, {".", true}},
      })
    );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({ {"!", true}, {"!", true}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true} })),
    vector<Sentence<TestToken>>({
        {{"!", true}, {"!", true}},
        {{"sentences"}, {"!", true}, {"!", true}},
        {{"Without"}, {"copies"}, {".", true}},
      })
    );
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSplitting);
  return 0;
}
