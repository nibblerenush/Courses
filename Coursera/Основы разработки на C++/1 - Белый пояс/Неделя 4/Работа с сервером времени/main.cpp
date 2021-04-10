#include <iostream>
#include <exception>
#include <string>
using namespace std;

string AskTimeServer() {
  /* ��� ������������ ������������ ���� ���, ����������� ��������� ��������� ���� ������:
     * ���������� ������� ���������� ��������
     * ������ ���������� system_error
     * ������ ������� ���������� � ����������.
  */
}

class TimeServer {
public:
  string GetCurrentTime() {
    /* ���������� ���� �����:
        * ���� AskTimeServer() ������� ��������, �������� ��� � last_fetched_time � �������
        * ���� AskTimeServer() ������� ���������� system_error, ������� ������� ��������
        ���� last_fetched_time
        * ���� AskTimeServer() ������� ������ ����������, ���������� ��� ������.
    */
    try
    {
      return last_fetched_time = AskTimeServer();
    }
    catch (const std::system_error& e)
    {
      return last_fetched_time;
    }
  }

private:
  string last_fetched_time = "00:00:00";
};

int main() {
  // ����� ���������� ������� AskTimeServer, ���������, ��� ��� ��� �������� ���������
  TimeServer ts;
  try {
    cout << ts.GetCurrentTime() << endl;
  }
  catch (exception& e) {
    cout << "Exception got: " << e.what() << endl;
  }
  return 0;
}
