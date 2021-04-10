#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::vector;

/*struct DisjointSetsElement {
	int size, parent, rank;
	
	DisjointSetsElement(int size = 0, int parent = -1, int rank = 0):
	    size(size), parent(parent), rank(rank) {}
};

struct DisjointSets {
	int size;
	int max_table_size;
	vector <DisjointSetsElement> sets;

	DisjointSets(int size): size(size), max_table_size(0), sets(size) {
		for (int i = 0; i < size; i++)
			sets[i].parent = i;
	}

	int getParent(int table) {
		// find parent and compress path
	}

	void merge(int destination, int source) {
		int realDestination = getParent(destination);
		int realSource = getParent(source);
		if (realDestination != realSource) {
			// merge two components
			// use union by rank heuristic
                        // update max_table_size
		}		
	}
};

int main() {
	int n, m;
	cin >> n >> m;

	DisjointSets tables(n);
	for (auto &table : tables.sets) {
		cin >> table.size;
		tables.max_table_size = max(tables.max_table_size, table.size);
	}

	for (int i = 0; i < m; i++) {
		int destination, source;
		cin >> destination >> source;
                --destination;
                --source;
		
		tables.merge(destination, source);
	        cout << tables.max_table_size << endl;
	}

	return 0;
}*/

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
