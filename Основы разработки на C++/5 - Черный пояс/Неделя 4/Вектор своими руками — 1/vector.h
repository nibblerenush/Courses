#include <cstddef>
#include <memory>
#include <new>
#include <utility>

template <typename T>
struct RawMemory
{
  T* buf = nullptr;
  std::size_t cp = 0;
  
  static T* Allocate(std::size_t n) {
    return static_cast<T*>(operator new(n * sizeof(T)));
  }
  
  static void Deallocate(T* buf) {
    operator delete(buf);
  }
  
  void Swap(RawMemory& other) {
    std::swap(buf, other.buf);
    std::swap(cp, other.cp);
  }

  RawMemory() = default;

  RawMemory(std::size_t n) {
    buf = Allocate(n);
    cp = n;
  }

  RawMemory(const RawMemory&) = delete;

  RawMemory(RawMemory&& other) {
    Swap(other);
  }

  RawMemory& operator=(const RawMemory&) = delete;

  RawMemory& operator=(RawMemory&& other) {
    Swap(other);
    return *this;
  } 

  ~RawMemory() {
    Deallocate(buf);
  }
};

template <typename T>
class Vector {
public:
  Vector() = default;

  Vector(size_t n) : m_raw_memory(n) {
    std::uninitialized_value_construct_n(m_raw_memory.buf, n);
    m_size = n;
  }

  Vector(const Vector& other) : m_raw_memory(other.m_size) {
    std::uninitialized_copy_n(other.m_raw_memory.buf, other.m_size, m_raw_memory.buf);
    m_size = other.m_size;
  }

  void Swap(Vector& other) {
    m_raw_memory.Swap(other.m_raw_memory);
    std::swap(m_size, other.m_size);
  }

  Vector(Vector&& other) {
    Swap(other);
  }

  ~Vector() {
    std::destroy_n(m_raw_memory.buf, m_size);
  }

  Vector& operator = (const Vector& other) {
    if (other.m_size > m_raw_memory.cp) {
      Vector tmp(other);
      Swap(tmp);
    }
    else {
      for (std::size_t i = 0; i < m_size && i < other.m_size; ++i) {
        *(m_raw_memory.buf + i) = *(other.m_raw_memory.buf + i);
      }

      if (m_size < other.m_size) {
        std::uninitialized_copy_n(
          other.m_raw_memory.buf + m_size,
          other.m_size - m_size,
          m_raw_memory.buf + m_size);
      }
      else if (m_size > other.m_size) {
        std::destroy_n(
          m_raw_memory.buf + other.m_size,
          m_size - other.m_size);
      }

      m_size = other.m_size;
    }

    return *this;
  }

  Vector& operator = (Vector&& other) noexcept {
    Swap(other);
    return *this;
  }

  void Reserve(size_t n) {
    if (n > m_raw_memory.cp) {
      RawMemory<T> new_raw_memory(n);
      std::uninitialized_move_n(m_raw_memory.buf, m_size, new_raw_memory.buf);
      std::destroy_n(m_raw_memory.buf, m_size);
      m_raw_memory.Swap(new_raw_memory);
    }
  }

  void Resize(size_t n) {
    Reserve(n);
    if (m_size < n) {
      std::uninitialized_value_construct_n(m_raw_memory.buf + m_size, n - m_size);
    }
    else if (m_size > n) {
      std::destroy_n(m_raw_memory.buf + n, m_size - n);
    }
    m_size = n;
  }

  void PushBack(const T& elem) {
    if (m_size == m_raw_memory.cp) {
      Reserve(m_size == 0 ? 1 : m_size * 2); 
    }
    new (m_raw_memory.buf + m_size) T(elem);
    m_size++;
  }

  void PushBack(T&& elem) {
    if (m_size == m_raw_memory.cp) {
      Reserve(m_size == 0 ? 1 : m_size * 2); 
    }
    new (m_raw_memory.buf + m_size) T(std::move(elem));
    m_size++;
  }

  template <typename ... Args>
  T& EmplaceBack(Args&&... args) {
    if (m_size == m_raw_memory.cp) {
      Reserve(m_size == 0 ? 1 : m_size * 2); 
    }
    auto elem = new (m_raw_memory.buf + m_size) T(std::forward<Args>(args)...);
    m_size++;
    return *elem;
  }

  void PopBack() {
    std::destroy_at(m_raw_memory.buf + m_size - 1);
    m_size--;
  }

  size_t Size() const noexcept {
    return m_size;
  }

  size_t Capacity() const noexcept {
    return m_raw_memory.cp;
  }

  const T& operator[](size_t i) const {
    return *(m_raw_memory.buf + i);
  }

  T& operator[](size_t i) {
    return *(m_raw_memory.buf + i);
  }

private:
  RawMemory<T> m_raw_memory;
  std::size_t m_size = 0;
};
