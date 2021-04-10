#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

struct Email {
  string from;
  string to;
  string body;
};

class Worker {
public:
  virtual ~Worker() = default;
  virtual void Process(unique_ptr<Email> email) = 0;
  virtual void Run() {
    // только первому worker-у в пайплайне нужно это имплементировать
    throw logic_error("Unimplemented");
  }

protected:
  // реализации должны вызывать PassOn, чтобы передать объект дальше
  // по цепочке обработчиков
  void PassOn(unique_ptr<Email> email) const {
    if (m_next) {
      m_next->Process(move(email));
    }
  }

public:
  void SetNext(unique_ptr<Worker> next) {
    m_next = move(next);
  }

private:
  unique_ptr<Worker> m_next = nullptr;
};

class Reader : public Worker {
public:
  Reader(istream& input) : m_input(input)
  {}

  void Process(unique_ptr<Email> email) override
  {
    PassOn(move(email));
  }
  
  void Run() override
  {
    while (m_input)
    {
      auto email = make_unique<Email>();
      if (!getline(m_input, email->from)) {
        break;
      }
      if (!getline(m_input, email->to)) {
        break;
      }
      if (!getline(m_input, email->body)) {
        break;
      }
      PassOn(move(email));
    }
  }

private:
  istream& m_input;
};


class Filter : public Worker {
public:
  using Function = function<bool(const Email&)>;

public:
  Filter(Function func) : m_func(move(func))
  {}

  void Process(unique_ptr<Email> email) override
  {
    if (m_func(*email)) {
      PassOn(move(email));
    }
  }

private:
  Function m_func;
};


class Copier : public Worker {
public:
  Copier(string to) : m_to(move(to))
  {}

  void Process(unique_ptr<Email> email) override
  {
    if (email->to == m_to) {
      PassOn(move(email));
    }
    else
    {
      auto new_email = make_unique<Email>(*email);
      new_email->to = m_to;
      PassOn(move(email));
      PassOn(move(new_email));
    }
  }

private:
  string m_to;
};


class Sender : public Worker {
public:
  Sender(ostream& output) : m_output(output)
  {}

  void Process(unique_ptr<Email> email) override
  {
    m_output << email->from << '\n' << email->to << '\n' << email->body << '\n';
    PassOn(move(email));
  }

private:
  ostream& m_output;
};

// реализуйте класс
class PipelineBuilder {
public:
  // добавляет в качестве первого обработчика Reader
  explicit PipelineBuilder(istream& in) : m_first_worker(make_unique<Reader>(in)), m_last_worker(m_first_worker.get())
  {}

  // добавляет новый обработчик Filter
  PipelineBuilder& FilterBy(Filter::Function filter)
  {
    SetLastWorker(make_unique<Filter>(move(filter)));
    return *this;
  }

  // добавляет новый обработчик Copier
  PipelineBuilder& CopyTo(string recipient)
  {
    SetLastWorker(make_unique<Copier>(move(recipient)));
    return *this;
  }

  // добавляет новый обработчик Sender
  PipelineBuilder& Send(ostream& out)
  {
    SetLastWorker(make_unique<Sender>(out));
    return *this;
  }

  // возвращает готовую цепочку обработчиков
  unique_ptr<Worker> Build()
  {
    return move(m_first_worker);
  }

private:
  void SetLastWorker(unique_ptr<Worker> new_worker)
  {
    Worker* new_worker_ptr = new_worker.get();
    m_last_worker->SetNext(move(new_worker));
    m_last_worker = new_worker_ptr;
  }

private:
  unique_ptr<Worker> m_first_worker;
  Worker* m_last_worker;
};


void TestSanity() {
  string input = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "ralph@example.com\n"
    "erich@example.com\n"
    "I do not make mistakes of that kind\n"
    );
  istringstream inStream(input);
  ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();

  pipeline->Run();

  string expectedOutput = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "erich@example.com\n"
    "richard@example.com\n"
    "Are you sure you pressed the right button?\n"
    );

  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  return 0;
}
