// Исключение этого типа должно генерироваться при обращении к "пустому" Optional в функции Value

#include <algorithm>
#include <utility>

struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
  // alignas нужен для правильного выравнивания блока памяти
  alignas(T) unsigned char data[sizeof(T)];
  T* object = nullptr;

public:
  Optional() = default;
  
  Optional(const T& elem) {
    object = new (data) T(elem);
  }

  Optional(T&& elem) {
    object = new (data) T(std::move(elem));
  }

  Optional(const Optional& other) {
    if (other.object) {
      object = new (data) T(*other.object);
    }
  }

  Optional(Optional&& other) {
    if (other.object) {
      object = new (data) T(std::move(*other.object));
    }
  }

  Optional& operator=(const T& elem) {
    if (object) {
      *object = elem;
    }
    else {
      object = new (data) T(elem);
    }

    return *this;
  }

  Optional& operator=(T&& elem) {
    if (object) {
      *object = std::move(elem);
    }
    else {
      object = new (data) T(std::move(elem));
    }

    return *this;
  }

  Optional& operator=(const Optional& other) {
    if (this != &other) {
      if (other.object) {
        if (object) {
          *object = *other.object;
        }
        else {
          object = new (data) T(*other.object);
        }
      }
      else {
        Reset();
      }
    }

    return *this;
  }

  Optional& operator=(Optional&& other) {
    if (this != &other) {
      if (other.object) {
        if (object) {
          *object = std::move(*other.object);
        }
        else {
          object = new (data) T(std::move(*other.object));
        }
      }
      else {
        Reset();
      }
    }

    return *this;
  }

  bool HasValue() const {
    return object != nullptr;
  }

  // Эти операторы не должны делать никаких проверок на пустоту.
  // Проверки остаются на совести программиста.
  T& operator*() {
    return *object;
  }

  const T& operator*() const {
    return *object;
  }

  T* operator->() {
    return object;
  }

  const T* operator->() const {
    return object;
  }

  // Генерирует исключение BadOptionalAccess, если объекта нет
  T& Value() {
    if (object) {
      return *object;
    }
    else {
      throw BadOptionalAccess{};
    }
  }

  const T& Value() const {
    if (object) {
      return *object;
    }
    else {
      throw BadOptionalAccess{};
    }
  }

  void Reset() {
    if (object) {
      object->~T();
      object = nullptr;
    }
  }

  ~Optional() {
    Reset();
  }
};
