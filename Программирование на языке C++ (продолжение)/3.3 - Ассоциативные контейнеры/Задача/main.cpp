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

// Base - ������� ����� ��������
// Result - ��� ������������� �������� ������������
// Commutative - ����, ������� ����������, ���
// ����������� ������������� (�.�. f(x,y) = f(y,x)).
template<class Base, class Result, bool Commutative>
struct Multimethod2
{
  // ������������� ���������� ������������
  // ��� ����� t1 � t2 �������� ����� typeid 
  // f - ��� ������� ��� �������������� ������
  // ����������� ��� ��������� �� Base 
  // � ������������ �������� ���� Result
  void addImpl(const std::type_info& t1, const std::type_info& t2, std::function<Result(Base*, Base*)> f) {
    m_t1_t2_f[std::type_index(t1)][std::type_index(t2)] = f;
  }

  // ���������, ���� �� ���������� ������������
  // ��� ����� �������� a � b
  bool hasImpl(Base* a, Base* b) const {
    // ���������� true, ���� ���������� ����
    // ���� �������� �������������, �� �����
    // ��������� ���� �� ���������� ��� b � �
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
  
  // ��������� ����������� � ��������
  // �� ���������� a � b
  Result call(Base* a, Base* b) const {
    // ���������� ��������� ���������� ����������
    // ������������ � a � b
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

// ������� ����� ������ (�����������)
struct Shape {
  virtual void func() {
    std::cout << "S\n";
  }
  virtual ~Shape() {}
};

// �������������
struct Rectangle : Shape {
  void func() override {
    std::cout << "R\n";
  }
  ~Rectangle() {}
};

// �����������
struct Triangle : Shape {
  void func() override {
    std::cout << "T\n";
  }
  ~Triangle() {}
};

// ������� ��� �������� ����������� ���� ���������������
bool is_intersect_r_r(Shape* a, Shape* b) {
  return true;
}

// ������� ��� �������� ����������� �������������� � ������������
bool is_intersect_r_t(Shape* a, Shape* b) {
  return true;
}

int main()
{
  // ����������� ��� ����������� Shape
  // ����������� bool � ���������� ������������� 
  Multimethod2<Shape, bool, true> is_intersect;

  // ��������� ���������� ������������ ��� ���� ���������������
  is_intersect.addImpl(typeid(Rectangle), typeid(Rectangle), is_intersect_r_r);

  // ��������� ���������� ������������ ��� �������������� � ������������
  is_intersect.addImpl(typeid(Rectangle), typeid(Triangle), is_intersect_r_t);

  // ������ ��� ������    
  Shape* s1 = new Triangle();
  Shape* s2 = new Rectangle();

  // ���������, ��� ���������� ��� s1 � s2 ����
  if (is_intersect.hasImpl(s1, s2))
  {
    // ���������� ������� is_intersect_r_t(s2, s1)
    bool res = is_intersect.call(s1, s2);

    // ���������: is_intersect_r_t �������,
    // ��� ������ ���������� ����� �������������
    // � ������ �����������, � ����� ���������
    // ���������� � �������� �������. 
    // ���� ���������� ������ �������������� 
    // �� ���� ������������
  }

  return 0;
}
