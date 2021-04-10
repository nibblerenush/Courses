#include <iostream>
#include <vector>

class DisjoinSet
{
public:
  DisjoinSet(std::size_t size) : m_rank(size + 1), m_parent(size + 1)
  {}

  void MakeSet(int i)
  {
    m_parent[i] = i;
    m_rank[i] = 0;
  }

  int Find(int i)
  {
    if (i != m_parent[i]) {
      m_parent[i] = Find(m_parent[i]);
    }
    return m_parent[i];
  }

  void Union(int i, int j)
  {
    int i_id = Find(i);
    int j_id = Find(j);

    if (i_id == j_id) {
      return;
    }

    if (m_rank[i_id] > m_rank[j_id]) {
      m_parent[j_id] = i_id;
    }
    else {
      m_parent[i_id] = j_id;
      if (m_rank[i_id] == m_rank[j_id]) {
        m_rank[j_id] += 1;
      }
    }
  }

private:
  std::vector<int> m_rank;
  std::vector<int> m_parent;
};

int main()
{
  int vars_count;
  int equals_count;
  int not_equals_count;
  std::cin >> vars_count >> equals_count >> not_equals_count;

  DisjoinSet disjoin_set(vars_count);
  for (int i = 1; i <= vars_count; ++i) {
    disjoin_set.MakeSet(i);
  }

  for (int i = 1; i <= equals_count; ++i) {
    int var1;
    int var2;
    std::cin >> var1 >> var2;
    disjoin_set.Union(var1, var2);
  }

  bool result = true;
  for (int i = 1; i <= not_equals_count; ++i) {
    int var1;
    int var2;
    std::cin >> var1 >> var2;
    if (disjoin_set.Find(var1) == disjoin_set.Find(var2)) {
      result = false;
      break;
    }
  }
  std::cout << result << std::endl;
  return 0;
}
