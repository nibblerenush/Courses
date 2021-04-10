#include <iostream>
#include <string>

struct NO {
  char no[1];
};

struct YES {
  char yes[2];
};

template <typename T, bool value>
struct HelperStruct {
  std::size_t get_size(T& object) {
    return object.size;
  }
};

template <typename T>
struct HelperStruct<T, true> {
  std::size_t get_size(T& object) {
    return object.size();
  }
};

template <typename T>
struct MainStruct {
  template <typename U, std::size_t(U::*)() const = &U::size>
  struct wrapper {};

  template <typename H>
  static YES check(wrapper<H>*);

  template <typename H>
  static NO check(...);

  static const bool value = sizeof(YES) == sizeof(check<T>(0));

  HelperStruct<T, value> helperStruct;

  std::size_t get_size(T& object) {
    return helperStruct.get_size(object);
  }
};

template <typename T>
std::size_t get_size(T& object) {
  MainStruct<T> mainStruct;
  return mainStruct.get_size(object);
}

int main() {
  std::string s{ "Hello" };
  size_t s_size = get_size(s); // 5, вызывается метод size()
  std::cout << s_size << std::endl;
  
  struct Struct {
    size_t size = 0;
  };
  
  Struct x{ 10 };
  size_t x_size = get_size(x); // 10, читается поле size
  std::cout << x_size << std::endl;

  return 0;
}
