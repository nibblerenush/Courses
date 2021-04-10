#include <algorithm>
#include <iostream>
#include <vector>

class DisjoinSet
{
public:
  DisjoinSet(std::size_t size) : m_rank(size + 1), m_parent(size + 1), m_record(size + 1), m_max_record(0)
  {}

  void MakeSet(int i, int record)
  {
    m_parent[i] = i;
    m_rank[i] = 0;
    m_record[i] = record;
    m_max_record = std::max(m_max_record, m_record[i]);
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

    int low_rank;
    int high_rank;
    if (m_rank[i_id] > m_rank[j_id])
    {
      m_parent[j_id] = i_id;
      low_rank = j_id;
      high_rank = i_id;
    }
    else
    {
      m_parent[i_id] = j_id;
      low_rank = i_id;
      high_rank = j_id;
      
      if (m_rank[i_id] == m_rank[j_id]) {
        m_rank[j_id] += 1;
      }
    }
    
    m_record[high_rank] += m_record[low_rank];
    m_max_record = std::max(m_max_record, m_record[high_rank]);
  }

  int Max() const {
    return m_max_record;
  }

private:
  std::vector<int> m_rank;
  std::vector<int> m_parent;
  std::vector<int> m_record;
  int m_max_record;
};

int main()
{
  int tables_count;
  int queries_count;
  std::cin >> tables_count >> queries_count;
  
  DisjoinSet disjoin_set(tables_count);
  for (int i = 1; i <= tables_count; ++i)
  {
    int record;
    std::cin >> record;
    disjoin_set.MakeSet(i, record);
  }

  for (int i = 1; i <= queries_count; ++i)
  {
    int dst;
    int src;
    std::cin >> dst >> src;
    disjoin_set.Union(dst, src);
    std::cout << disjoin_set.Max() << '\n';
  }

  return 0;
}
