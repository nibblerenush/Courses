#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate();
  T* TryAllocate();

  void Deallocate(T* object);

  ~ObjectPool();

private:
  std::set<T*> m_busy_objects;
  std::queue<T*> m_free_objects;
};

template <typename T>
T* ObjectPool<T>::Allocate()
{
  T* result;
  if (!m_free_objects.empty())
  {
    result = m_free_objects.front();
    m_free_objects.pop();
  }
  else
  {
    result = new T;
  }
  m_busy_objects.insert(result);
  return result;
}

template <typename T>
T* ObjectPool<T>::TryAllocate()
{
  T* result;
  if (!m_free_objects.empty())
  {
    result = m_free_objects.front();
    m_free_objects.pop();
    m_busy_objects.insert(result);
  }
  else
  {
    result = nullptr;
  }
  return result;
}

template <typename T>
void ObjectPool<T>::Deallocate(T* object)
{
  if (!m_busy_objects.erase(object))
  {
    throw std::invalid_argument("");
  }
  m_free_objects.push(object);
}

template <typename T>
ObjectPool<T>::~ObjectPool()
{
  for (T* object : m_busy_objects)
  {
    delete object;
  }

  while (!m_free_objects.empty())
  {
    T* object = m_free_objects.front();
    m_free_objects.pop();
    delete object;
  }
}

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
