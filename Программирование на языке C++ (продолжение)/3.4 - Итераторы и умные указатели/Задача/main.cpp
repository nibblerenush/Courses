#include <algorithm>
#include <cmath>
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <vector>
#include <list>

template<class T>
class VectorList {
private:
  using VectT = std::vector<T>;
  using ListT = std::list<VectT>;

  using VectIterT = typename std::vector<T>::const_iterator;
  using ListIterT = typename std::list<VectT>::const_iterator;

public:
  using value_type = T;

  VectorList() = default;
  VectorList(VectorList const&) = default;
  VectorList(VectorList&&) = default;
  
  VectorList& operator=(VectorList&&) = default;
  VectorList& operator=(VectorList const&) = default;
  
  // метод, который будет использоваться для заполнения VectorList
  // гарантирует, что в списке не будет пустых массивов
  template<class It>
  void append(It p, It q); // определена снаружи
  
  bool empty() const { return size() == 0; }
  
  // определите метод size
  size_t size() const {
    size_t result = 0;
    for (const VectT& vect : data_) {
      result += vect.size();
    }
    return result;
  }

  // определите const_iterator
  struct const_iterator : public std::iterator<std::bidirectional_iterator_tag, T, int, const T*, const T&> {
  public:
    // default
    const_iterator() = default;

    // copy
    const_iterator(const const_iterator&) = default;

    // custom
    const_iterator(const ListT* data, ListIterT list_iter) : m_data(data), m_list_iter(list_iter), m_vect_iter() {
      if (m_list_iter == m_data->end()) {
        return;
      }

      if (m_list_iter == m_data->begin()) {
        m_vect_iter = m_list_iter->begin();
      }
      else {
        assert(false);
      }
    }

    const_iterator& operator++ () {
      ++m_vect_iter;
      
      if (m_vect_iter == m_list_iter->end()) {
        ++m_list_iter;
        
        if (m_list_iter == m_data->end()) {
          m_vect_iter = VectIterT{};
        }
        else {
          m_vect_iter = m_list_iter->begin();
        }
      }

      return *this;
    }

    const_iterator operator++ (int) {
      const_iterator result = *this;
      ++(*this);
      return result;
    }

    const_iterator& operator--() {
      if (m_list_iter == m_data->end()) {
        m_list_iter = std::prev(m_data->end());
        m_vect_iter = std::prev(m_list_iter->end());
      }
      else {
        if (m_vect_iter == m_list_iter->begin()) {
          --m_list_iter;
          m_vect_iter = std::prev(m_list_iter->end());
        }
        else {
          --m_vect_iter;
        }
      }
      
      return *this;
    }

    const_iterator operator-- (int) {
      const_iterator result = *this;
      --(*this);
      return result;
    }
    
    bool operator== (const const_iterator& it) const {
      return m_list_iter == it.m_list_iter && m_vect_iter == it.m_vect_iter;
    }

    bool operator!= (const const_iterator& it) const {
      return !(*this == it);
    }

    const T& operator*() const {
      return *m_vect_iter;
    }

    const T* operator->() const {
      return &(*m_vect_iter);
    }

  private:
    const ListT* m_data = nullptr;
    ListIterT m_list_iter;
    VectIterT m_vect_iter;
  };
  
  // определите методы begin / end
  const_iterator begin() const {
    return const_iterator(&data_, data_.begin());
  }
  
  const_iterator end() const {
    return const_iterator(&data_, data_.end());
  }

  // определите const_reverse_iterator
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  
  // определите методы rbegin / rend
  const_reverse_iterator rbegin() const { 
    return const_reverse_iterator(this->end());
  }
  
  const_reverse_iterator rend() const {
    return const_reverse_iterator(this->begin());
  }

private:
  ListT data_;
};

template <typename T>
template <typename It>
void VectorList<T>::append(It p, It q) {
  if (p != q) {
    data_.push_back(VectT(p, q));
  }
}

int main() {
  VectorList<char> vlist;
  
  std::vector<char> v1;
  v1.push_back('A');
  v1.push_back('B');
  v1.push_back('C');
  
  std::vector<char> v2;
  v2.push_back('D');
  v2.push_back('E');
  v2.push_back('F');
  v2.push_back('G');
  vlist.append(v1.begin(), v1.end());
  vlist.append(v2.begin(), v2.end());

  auto i = vlist.begin();

  std::cout << "Size is " << vlist.size() << std::endl;
  std::cout << "begin is " << *i << std::endl;
  std::cout << "std::distance(begin,end)﻿ " << (std::distance(vlist.begin(), vlist.end())) << std::endl;
  std::cout << "*(++begin) == 'B'? " << (*++vlist.begin() == 'B') << std::endl;
  std::cout << "*(++begin) == 'A'? " << (*++vlist.begin() == 'A') << std::endl;
  std::cout << std::endl;

  std::cout << "Test ++i" << std::endl;
  for (i = vlist.begin(); i != vlist.end(); ++i)
    std::cout << *i << " ";
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "Test i++" << std::endl;
  for (i = vlist.begin(); i != vlist.end(); i++)
    std::cout << *i << " ";
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "Test --i" << std::endl;
  for (i = vlist.end(); i != vlist.begin();)
    std::cout << *--i << " ";
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "Test i--" << std::endl;
  for (i = vlist.end(); i != vlist.begin();) {
    i--;
    std::cout << *i << " ";
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << std::endl;
  auto j = vlist.rbegin();
  std::cout << "rbegin is " << *j << std::endl;
  j = --vlist.rend();
  std::cout << "--rend is " << *j << std::endl;

  std::cout << "Test reverse_const_iterator ++" << std::endl;
  for (j = vlist.rbegin(); j != vlist.rend(); ++j)
    std::cout << *j << " ";
  std::cout << std::endl;

  return 0;
}
