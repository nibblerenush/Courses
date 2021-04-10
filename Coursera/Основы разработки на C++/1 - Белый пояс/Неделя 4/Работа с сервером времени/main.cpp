#include <iostream>
#include <exception>
#include <string>
using namespace std;

string AskTimeServer() {
  /* ƒл€ тестировани€ повставл€йте сюда код, реализующий различное поведение этой функии:
     * нормальный возврат строкового значени€
     * выброс исключени€ system_error
     * выброс другого исключени€ с сообщением.
  */
}

class TimeServer {
public:
  string GetCurrentTime() {
    /* –еализуйте этот метод:
        * если AskTimeServer() вернула значение, запишите его в last_fetched_time и верните
        * если AskTimeServer() бросила исключение system_error, верните текущее значение
        пол€ last_fetched_time
        * если AskTimeServer() бросила другое исключение, пробросьте его дальше.
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
  // ћен€€ реализацию функции AskTimeServer, убедитесь, что это код работает корректно
  TimeServer ts;
  try {
    cout << ts.GetCurrentTime() << endl;
  }
  catch (exception& e) {
    cout << "Exception got: " << e.what() << endl;
  }
  return 0;
}
