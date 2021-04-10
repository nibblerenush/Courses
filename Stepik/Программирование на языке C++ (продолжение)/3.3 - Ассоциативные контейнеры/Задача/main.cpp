#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

#include <functional>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

// Base - базовый класс иерархии
// Result - тип возвращаемого значения мультиметода
// Commutative - флаг, который показывает, что
// мультиметод коммутативный (т.е. f(x,y) = f(y,x)).
template<class Base, class Result, bool Commutative>
struct Multimethod2
{
  // устанавливает реализацию мультиметода
  // для типов t1 и t2 заданных через typeid 
  // f - это функция или функциональный объект
  // принимающий два указателя на Base 
  // и возвращающий значение типа Result
  void addImpl(const std::type_info& t1, const std::type_info& t2, std::function<Result(Base*, Base*)> f) {
    m_t1_t2_f[std::type_index(t1)][std::type_index(t2)] = f;
  }

  // проверяет, есть ли реализация мультиметода
  // для типов объектов a и b
  bool hasImpl(Base* a, Base* b) const {
    // возвращает true, если реализация есть
    // если операция коммутативная, то нужно
    // проверить есть ли реализация для b и а
    std::type_index t1(typeid(*a));
    std::type_index t2(typeid(*b));
    
    if (m_t1_t2_f.count(t1) && m_t1_t2_f.at(t1).count(t2)) {
      return true;
    }
    
    if (!Commutative) {
      return false;
    }
    
    return m_t1_t2_f.count(t2) && m_t1_t2_f.at(t2).count(t1);
  }
  
  // Применяет мультиметод к объектам
  // по указателям a и b
  Result call(Base* a, Base* b) const {
    // возвращает результат применения реализации
    // мультиметода к a и b
    std::type_index t1(typeid(*a));
    std::type_index t2(typeid(*b));
    
    if (!Commutative) {
      return m_t1_t2_f.at(t1).at(t2)(a, b);
    }
    
    if (m_t1_t2_f.count(t1) && m_t1_t2_f.at(t1).count(t2)) {
      return m_t1_t2_f.at(t1).at(t2)(a, b);
    }
    
    return m_t1_t2_f.at(t2).at(t1)(b, a);
  }

private:
  std::unordered_map<std::type_index,
    std::unordered_map<std::type_index,
      std::function<Result(Base*, Base*)>
    >
  > m_t1_t2_f;
};

// базовый класс фигуры (полиморфный)
struct Shape {
  virtual void func() {
    std::cout << "S\n";
  }
  virtual ~Shape() {}
};

// прямоугольник
struct Rectangle : Shape {
  void func() override {
    std::cout << "R\n";
  }
  ~Rectangle() {}
};

// треугольник
struct Triangle : Shape {
  void func() override {
    std::cout << "T\n";
  }
  ~Triangle() {}
};

// функция для проверки пересечения двух прямоугольников
bool is_intersect_r_r(Shape* a, Shape* b) {
  return true;
}

// функция для проверки пересечения прямоугольника и треугольника
bool is_intersect_r_t(Shape* a, Shape* b) {
  return true;
}

int main()
{
  // мультиметод для наследников Shape
  // возращающий bool и являющийся коммутативным 
  Multimethod2<Shape, bool, true> is_intersect;

  // добавляем реализацию мультиметода для двух прямоугольников
  is_intersect.addImpl(typeid(Rectangle), typeid(Rectangle), is_intersect_r_r);

  // добавляем реализацию мультиметода для прямоугольника и треугольника
  is_intersect.addImpl(typeid(Rectangle), typeid(Triangle), is_intersect_r_t);

  // создаём две фигуры    
  Shape* s1 = new Triangle();
  Shape* s2 = new Rectangle();

  // проверяем, что реализация для s1 и s2 есть
  if (is_intersect.hasImpl(s1, s2))
  {
    // вызывается функция is_intersect_r_t(s2, s1)
    bool res = is_intersect.call(s1, s2);

    // Замечание: is_intersect_r_t ожидает,
    // что первым аргументом будет прямоугольник
    // а вторым треугольник, а здесь аргументы
    // передаются в обратном порядке. 
    // ваша реализация должна самостоятельно 
    // об этом позаботиться
  }

  return 0;
}
