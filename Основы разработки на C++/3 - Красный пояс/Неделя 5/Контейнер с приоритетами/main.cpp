#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:

  using Id = int;

  // �������� ������ � ������� �����������
  // � ������� ����������� � ������� ��� �������������
  Id Add(T object);

  // �������� ��� �������� ��������� [range_begin, range_end)
  // � ������� �����������, ������� �������� �� ��������������
  // � �������� [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
    IdOutputIt ids_begin);

  // ����������, ����������� �� ������������� ������-����
  // ����������� � ���������� �������
  bool IsValid(Id id) const;

  // �������� ������ �� ��������������
  const T& Get(Id id) const;

  // ��������� ��������� ������� �� 1
  void Promote(Id id);

  // �������� ������ � ������������ ����������� � ��� ���������
  pair<const T&, int> GetMax() const;

  // ���������� GetMax, �� ������� ������� �� ����������
  pair<T, int> PopMax();
  
  struct ObjectSort
  {
    int priority;
    Id id;
  };
  
  struct ObjectSortCmp
  {
    bool operator()(const ObjectSort& lhs, const ObjectSort& rhs) const
    {
      return std::tie(lhs.priority, lhs.id) < std::tie(rhs.priority, rhs.id);
    }
  };

private:
  Id m_max_object_id = -1;
  std::map<ObjectSort, T, ObjectSortCmp> m_sort_objects;
  std::map<Id, typename std::map<ObjectSort, T, ObjectSortCmp>::iterator> m_id_iter;
};

template <typename T>
typename PriorityCollection<T>::Id PriorityCollection<T>::Add(T object)
{
  auto [iter, is_insert] = m_sort_objects.emplace(ObjectSort{ 0, ++m_max_object_id }, std::move(object));
  m_id_iter[m_max_object_id] = iter;
  return m_max_object_id;
}

template <typename T>
template <typename ObjInputIt, typename IdOutputIt>
void PriorityCollection<T>::Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin)
{
  for (auto iter = range_begin; iter != range_end; ++iter)
  {
    *(ids_begin++) = Add(std::move(*iter));
  }
}

template <typename T>
bool PriorityCollection<T>::IsValid(Id id) const
{
  return m_id_iter.find(id) != m_id_iter.end();
}

template <typename T>
const T& PriorityCollection<T>::Get(Id id) const
{
  return m_id_iter.at(id)->second;
}

template <typename T>
void PriorityCollection<T>::Promote(Id id)
{
  // find id
  auto id_iter = m_id_iter.at(id);
  
  // update priority
  T object = std::move(id_iter->second);
  ObjectSort objectSort = id_iter->first;
  objectSort.priority++;

  // erase old priority
  m_sort_objects.erase(id_iter);

  // insert new priority
  auto [iter, is_insert] = m_sort_objects.emplace(objectSort, std::move(object));
  m_id_iter[id] = iter;
}

template <typename T>
std::pair<const T&, int> PriorityCollection<T>::GetMax() const
{
  return { m_sort_objects.rbegin()->second, m_sort_objects.rbegin()->first.priority };
}

template <typename T>
std::pair<T, int> PriorityCollection<T>::PopMax()
{
  std::pair<T, int> result{ std::move(m_sort_objects.rbegin()->second), m_sort_objects.rbegin()->first.priority };
  m_id_iter.erase(m_sort_objects.rbegin()->first.id);
  m_sort_objects.erase(std::prev(m_sort_objects.end()));
  return result;
}

class StringNonCopyable : public string {
public:
  using string::string;  // ��������� ������������ ������������ ������
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

void MyTest()
{
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");
  const auto brown_id = strings.Add("brown");
  const auto black_id = strings.Add("black");

  ASSERT(strings.IsValid(white_id));
  ASSERT(strings.IsValid(yellow_id));
  ASSERT(strings.IsValid(red_id));
  ASSERT(strings.IsValid(brown_id));
  ASSERT(strings.IsValid(black_id));

  strings.Promote(white_id);
  strings.Promote(white_id);

  {
    auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 2);
    ASSERT(!strings.IsValid(white_id));
  }

  {
    const auto& item = strings.Get(yellow_id);
    ASSERT_EQUAL(item, "yellow");
  }

  {
    std::pair<const StringNonCopyable&, int> max = strings.GetMax();
    ASSERT_EQUAL(max.first, "black");
    ASSERT_EQUAL(max.second, 0);
  }
}

int main()
{
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  RUN_TEST(tr, MyTest);
  return 0;
}
