#include <tuple>
#include <vector>

template <typename T>
class Deque
{
public:
  bool Empty() const;
  std::size_t Size() const;
  T& operator [] (std::size_t index);
  const T& operator [] (std::size_t index) const;
  T& At(std::size_t index);
  const T& At(std::size_t index) const;
  T& Front();
  const T& Front() const;
  T& Back();
  const T& Back() const;
  void PushFront(const T& item);
  void PushBack(const T& item);
private:
  const T& SquareBrackets(std::size_t index) const;
  const T& AtMethod(std::size_t index) const;
  std::tuple<const std::vector<T>&, std::size_t> GetRealIndex(std::size_t index) const;
  const T& FrontMethod() const;
  const T& BackMethod() const;
private:
  std::vector<T> m_front;
  std::vector<T> m_back;
};

template <typename T>
bool Deque<T>::Empty() const
{
  return m_front.empty() && m_back.empty();
}

template <typename T>
std::size_t Deque<T>::Size() const
{
  return m_front.size() + m_back.size();
}

template <typename T>
T& Deque<T>::operator [] (std::size_t index)
{
  return const_cast<T&>(SquareBrackets(index));
}

template <typename T>
const T& Deque<T>::operator [] (std::size_t index) const
{
  return SquareBrackets(index);
}

template <typename T>
T& Deque<T>::At(std::size_t index)
{
  return const_cast<T&>(AtMethod(index));
}

template <typename T>
const T& Deque<T>::At(std::size_t index) const
{
  return AtMethod(index);
}

template <typename T>
void Deque<T>::PushFront(const T& item)
{
  m_front.push_back(item);
}

template <typename T>
void Deque<T>::PushBack(const T& item)
{
  m_back.push_back(item);
}

template <typename T>
T& Deque<T>::Front()
{
  return const_cast<T&>(FrontMethod());
}

template <typename T>
const T& Deque<T>::Front() const
{
  return FrontMethod();
}

template <typename T>
T& Deque<T>::Back()
{
  return const_cast<T&>(BackMethod());
}

template <typename T>
const T& Deque<T>::Back() const
{
  return BackMethod();
}

template <typename T>
const T& Deque<T>::SquareBrackets(std::size_t index) const
{
  auto [real_vector, real_index] = GetRealIndex(index);
  return real_vector[real_index];
}

template <typename T>
const T& Deque<T>::AtMethod(std::size_t index) const
{
  auto [real_vector, real_index] = GetRealIndex(index);
  return real_vector.at(real_index);
}

template <typename T>
const T& Deque<T>::FrontMethod() const
{
  if (m_front.empty())
  {
    return m_back[0];
  }
  else
  {
    return m_front.back();
  }
}

template <typename T>
const T& Deque<T>::BackMethod() const
{
  if (m_back.empty())
  {
    return m_front[0];
  }
  else
  {
    return m_back.back();
  }
}

template <typename T>
std::tuple<const std::vector<T>&, std::size_t> Deque<T>::GetRealIndex(std::size_t index) const
{
  if (index < m_front.size())
  {
    index = m_front.size() - 1 - index;
    return { m_front, index };
  }
  else
  {
    index -= m_front.size();
    return { m_back, index };
  }
}
