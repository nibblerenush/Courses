#include "test_runner.h"
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class Table
{
public:
  Table(std::size_t num_rows, std::size_t num_cols);
  std::vector<T>& operator [] (std::size_t index);
  const std::vector<T>& operator [] (std::size_t index) const;
  void Resize(std::size_t num_rows, std::size_t num_cols);
  std::pair<std::size_t, std::size_t> Size() const;

private:
  std::vector<std::vector<T>> m_table;
};

template <typename T>
Table<T>::Table(std::size_t num_rows, std::size_t num_cols)
{
  Resize(num_rows, num_cols);
}

template <typename T>
std::vector<T>& Table<T>::operator [] (std::size_t index)
{
  return m_table.at(index);
}

template <typename T>
const std::vector<T>& Table<T>::operator [] (std::size_t index) const
{
  return m_table.at(index);
}

template <typename T>
void Table<T>::Resize(std::size_t num_rows, std::size_t num_cols)
{
  if (num_rows > 0 && num_cols > 0)
  {
    m_table.resize(num_rows);
    for (auto& row : m_table)
    {
      row.resize(num_cols);
    }
  }
  else
  {
    m_table.clear();
  }
}

template <typename T>
std::pair<std::size_t, std::size_t> Table<T>::Size() const
{
  if (!m_table.empty())
  {
    return { m_table.size(), m_table.at(0).size() };
  }
  else
  {
    return { 0, 0 };
  }
}

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  Table<int> t(2, 2);
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
