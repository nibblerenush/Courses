#pragma once

#include <algorithm>
#include <cstdlib>

template <typename T>
class SimpleVector
{
public:
  SimpleVector();
  explicit SimpleVector(std::size_t size);
  ~SimpleVector();

  T& operator[](std::size_t index);

  T* begin();
  T* end();

  std::size_t Size() const;
  std::size_t Capacity() const;
  void PushBack(const T& value);

private:
  T* m_data;
  std::size_t m_size;
  std::size_t m_capacity;
};

template <typename T>
SimpleVector<T>::SimpleVector(): m_data(nullptr), m_size(0), m_capacity(0)
{}

template <typename T>
SimpleVector<T>::SimpleVector(std::size_t size): m_data(new T[size]), m_size(size), m_capacity(size)
{}

template <typename T>
SimpleVector<T>::~SimpleVector()
{
  delete[] m_data;
}

template <typename T>
T& SimpleVector<T>::operator[](std::size_t index)
{
  return m_data[index];
}

template <typename T>
T* SimpleVector<T>::begin()
{
  return m_data;
}

template <typename T>
T* SimpleVector<T>::end()
{
  return m_data + m_size;
}

template <typename T>
std::size_t SimpleVector<T>::Size() const
{
  return m_size;
}

template <typename T>
std::size_t SimpleVector<T>::Capacity() const
{
  return m_capacity;
}

template <typename T>
void SimpleVector<T>::PushBack(const T& value)
{
  if (m_size == m_capacity)
  {
    if (m_size == 0)
    {
      m_capacity = 1;
      m_data = new T[m_capacity];
    }
    else
    {
      m_capacity *= 2;
      T* new_data = new T[m_capacity];
      std::copy(m_data, m_data + m_size, new_data);
      delete[] m_data;
      m_data = new_data;
    }
  }
  m_data[m_size++] = value;
}
