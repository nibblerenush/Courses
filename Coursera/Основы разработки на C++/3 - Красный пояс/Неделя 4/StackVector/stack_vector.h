#pragma once

#include <array>
#include <stdexcept>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
  explicit StackVector(size_t a_size = 0);

  T& operator[](size_t index);
  const T& operator[](size_t index) const;

  typename std::array<T, N>::iterator begin();
  typename std::array<T, N>::iterator end();
  typename std::array<T, N>::const_iterator begin() const;
  typename std::array<T, N>::const_iterator end() const;

  size_t Size() const;
  size_t Capacity() const;

  void PushBack(const T& value);
  T PopBack();

private:
  std::size_t m_size;
  const std::size_t m_capacity;
  std::array<T, N> m_array;
};

template <typename T, std::size_t N>
StackVector<T, N>::StackVector(std::size_t size) : m_size(size), m_capacity(N)
{
  if (m_size > m_capacity)
  {
    throw std::invalid_argument("");
  }
}

template <typename T, std::size_t N>
T& StackVector<T, N>::operator[](std::size_t index)
{
  return m_array[index];
}

template <typename T, std::size_t N>
const T& StackVector<T, N>::operator[](std::size_t index) const
{
  return m_array[index];
}

template <typename T, std::size_t N>
typename std::array<T, N>::iterator StackVector<T, N>::begin()
{
  return m_array.begin();
}

template <typename T, std::size_t N>
typename std::array<T, N>::iterator StackVector<T, N>::end()
{
  return m_array.begin() + m_size;
}

template <typename T, std::size_t N>
typename std::array<T, N>::const_iterator StackVector<T, N>::begin() const
{
  return m_array.begin();
}

template <typename T, std::size_t N>
typename std::array<T, N>::const_iterator StackVector<T, N>::end() const
{
  return m_array.begin() + m_size;
}

template <typename T, std::size_t N>
std::size_t StackVector<T, N>::Size() const
{
  return m_size;
}

template <typename T, std::size_t N>
std::size_t StackVector<T, N>::Capacity() const
{
  return m_capacity;
}

template <typename T, std::size_t N>
void StackVector<T, N>::PushBack(const T& value)
{
  if (m_size == m_capacity)
  {
    throw std::overflow_error("");
  }
  else
  {
    m_array[m_size++] = value;
  }
}

template <typename T, std::size_t N>
T StackVector<T, N>::PopBack()
{
  if (m_size == 0)
  {
    throw std::underflow_error("");
  }
  else
  {
    return m_array[m_size-- - 1];
  }
}
