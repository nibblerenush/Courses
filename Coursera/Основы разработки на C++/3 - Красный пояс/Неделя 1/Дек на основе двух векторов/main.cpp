#include "test_runner.h"
#include "deque.h"
#include <iostream>

void TestSquareBrackets()
{
  {
    Deque<int> deque;
    deque.PushBack(1);
    deque.PushBack(2);
    deque.PushBack(3);
    deque.PushFront(0);
    deque.PushFront(-1);
    deque.PushFront(-2);
    deque.PushFront(-3);

    AssertEqual(deque[0], -3, "");
    AssertEqual(deque[1], -2, "");
    AssertEqual(deque[2], -1, "");
    AssertEqual(deque[3], 0, "");
    AssertEqual(deque[4], 1, "");
    AssertEqual(deque[5], 2, "");
    AssertEqual(deque[6], 3, "");

    AssertEqual(deque.Back(), 3, "");
    AssertEqual(deque.Front(), -3, "");
  }

  {
    Deque<int> deque;
    deque.PushBack(1);
    deque.PushBack(2);
    deque.PushBack(3);

    AssertEqual(deque[0], 1, "");
    AssertEqual(deque[1], 2, "");
    AssertEqual(deque[2], 3, "");

    AssertEqual(deque.Back(), 3, "");
    AssertEqual(deque.Front(), 1, "");
  }

  {
    Deque<int> deque;
    deque.PushFront(0);
    deque.PushFront(-1);
    deque.PushFront(-2);
    deque.PushFront(-3);

    AssertEqual(deque[0], -3, "");
    AssertEqual(deque[1], -2, "");
    AssertEqual(deque[2], -1, "");
    AssertEqual(deque[3], 0, "");

    AssertEqual(deque.Back(), 0, "");
    AssertEqual(deque.Front(), -3, "");
  }

  {
    Deque<int> deque;
    deque.PushBack(1);
    deque.PushBack(2);
    deque.PushBack(3);
    deque.PushFront(0);
    deque.PushFront(-1);
    deque.PushFront(-2);
    deque.PushFront(-3);

    AssertEqual(deque.At(0), -3, "");
    AssertEqual(deque.At(1), -2, "");
    AssertEqual(deque.At(2), -1, "");
    AssertEqual(deque.At(3), 0, "");
    AssertEqual(deque.At(4), 1, "");
    AssertEqual(deque.At(5), 2, "");
    AssertEqual(deque.At(6), 3, "");
  }
}

void Func()
{
  ASSERT_EQUAL(1, 2);
}

void TestAll()
{
  TestRunner tr;
  RUN_TEST(tr, TestSquareBrackets);
}

int main()
{
  TestAll();
  Deque<int> deque;
  deque.PushFront(10);
  std::cout << deque.Front() << std::endl;
  std::cout << deque.Back() << std::endl;

  const Deque<int> deque_const;
  deque_const.Back();
  deque_const.Front();
  deque_const.At(0);
  deque_const[0];
  return 0;
}
