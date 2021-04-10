#include "test_runner.h"
#include <sstream>
#include <string>
using namespace std;

class Logger {
public:
  explicit Logger(ostream& output_stream) : os(output_stream) {
  }

  void SetLogLine(bool value) { log_line = value; }
  void SetLogFile(bool value) { log_file = value; }

  bool GetLogLile() { return log_line; }
  bool GetLogFile() { return log_file; }

  void Log(const string& message)
  {
    os << message << endl;
  }

private:
  ostream& os;
  bool log_line = false;
  bool log_file = false;
};

void Log(Logger logger, const std::string& message, const std::string& file, int line)
{
  std::ostringstream oss;
  if (logger.GetLogFile())
  {
    oss << file;
    if (logger.GetLogLile())
    {
      oss << ':' << line;
    }
    oss << ' ';
  }
  else if (logger.GetLogLile())
  {
    oss << "Line " << line << ' ';
  }
  oss << message;
  logger.Log(oss.str());
}

#define LOG(logger, message) Log(logger, message, __FILE__, __LINE__)

void TestLog() {
  /* ��� ��������� ����-������ � ���� ������ ��� ����� ����������� ����������
   * ������ ����� � ��������� �������� (��. ���������� expected ����). ����
   * �� ��������� �����-�� ��� ���� ������� TestLog, �� ��� ������ ����� ��������,
   * � ��� ���� �������� ������. ��� ��������.
   *
   * ����� ����� ��������, �� ���������� ����������� ������ #line
   * (http://en.cppreference.com/w/cpp/preprocessor/line), ������� ���������
   * �������������� ����� ������, � ����� ��� �����. ��������� ���, ������
   * ����� ������ ������� TestLog ����� ����������� ���������� �� ����, �����
   * ��� �� ��������� ����� ���*/
#line 1 "logger.cpp"

  ostringstream logs;
  Logger l(logs);
  LOG(l, "hello");

  l.SetLogFile(true);
  LOG(l, "hello");

  l.SetLogLine(true);
  LOG(l, "hello");

  l.SetLogFile(false);
  LOG(l, "hello");

  string expected = "hello\n";
  expected += "logger.cpp hello\n";
  expected += "logger.cpp:10 hello\n";
  expected += "Line 13 hello\n";
  ASSERT_EQUAL(logs.str(), expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLog);
}
