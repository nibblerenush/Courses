#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

// Реализуйте этот класс
class Database {
public:
  bool Put(const Record& record)
  {
    // Id not found
    if (m_id_rec.find(record.id) == m_id_rec.end())
    {
      m_nodes.push_front({ record });
      auto node = m_nodes.begin();

      node->id_rec_iter = m_id_rec.emplace(record.id, node).first;
      node->timestamp_rec_iter = m_timestamp_rec.emplace(record.timestamp, node);
      node->karma_rec_iter = m_karma_rec.emplace(record.karma, node);
      node->user_rec_iter = m_user_rec.emplace(record.user, node);
      return true;
    }
    return false;
  }

  const Record* GetById(const string& id) const
  {
    // Id not found
    if (m_id_rec.find(id) == m_id_rec.end())
    {
      return nullptr;
    }
    return &m_id_rec.at(id)->record;
  }

  bool Erase(const string& id)
  {
    // Id not found
    if (m_id_rec.find(id) == m_id_rec.end())
    {
      return false;
    }

    auto node_iter = m_id_rec.at(id);
    m_id_rec.erase(node_iter->id_rec_iter);
    m_timestamp_rec.erase(node_iter->timestamp_rec_iter);
    m_karma_rec.erase(node_iter->karma_rec_iter);
    m_user_rec.erase(node_iter->user_rec_iter);
    m_nodes.erase(node_iter);
    return true;
  }

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const
  {
    auto iter_low = m_timestamp_rec.lower_bound(low);
    auto iter_high = m_timestamp_rec.upper_bound(high);

    for (auto iter = iter_low; iter != iter_high; ++iter)
    {
      if (!callback(iter->second->record)) {
        break;
      }
    }
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const
  {
    auto iter_low = m_karma_rec.lower_bound(low);
    auto iter_high = m_karma_rec.upper_bound(high);

    for (auto iter = iter_low; iter != iter_high; ++iter)
    {
      if (!callback(iter->second->record)) {
        break;
      }
    }
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const
  {
    auto iter_pair = m_user_rec.equal_range(user);

    for (auto iter = iter_pair.first; iter != iter_pair.second; ++iter)
    {
      if (!callback(iter->second->record)) {
        break;
      }
    }
  }

private:
  struct Node
  {
    Record record;
    map<string, list<Node>::iterator>::iterator id_rec_iter;
    multimap<int, list<Node>::iterator>::iterator timestamp_rec_iter;
    multimap<int, list<Node>::iterator>::iterator karma_rec_iter;
    multimap<string, list<Node>::iterator>::iterator user_rec_iter;
  };

  list<Node> m_nodes;
  map<string, list<Node>::iterator> m_id_rec;
  multimap<int, list<Node>::iterator> m_timestamp_rec;
  multimap<int, list<Node>::iterator> m_karma_rec;
  multimap<string, list<Node>::iterator> m_user_rec;
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({ "id1", "Hello there", "master", 1536107260, good_karma + 1 });
  db.Put({ "id2", "Hello there", "master", 1536107260, good_karma });
  db.Put({ "id3", "Hello there", "master", 1536107260, good_karma });
  db.Put({ "id4", "Hello there", "master", 1536107260, good_karma });
  db.Put({ "id5", "O>>-<", "general2", 1536107260, bad_karma });
  db.Put({ "id6", "O>>-<", "general2", 1536107260, bad_karma });
  db.Put({ "id7", "O>>-<", "general2", 1536107260, bad_karma });
  db.Put({ "id8", "O>>-<", "general2", 1536107260, bad_karma });
  db.Put({ "id9", "O>>-<", "general2", 1536107260, bad_karma + 5 });
  db.Put({ "id10", "O>>-<", "general2", 1536107260, bad_karma + 3 });
  db.Put({ "id11", "O>>-<", "general2", 1536107260, bad_karma - 3 });
  db.Put({ "id12", "O>>-<", "general2", 1536107260, bad_karma - 5 });

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(9, count);
}

void TestSameUser() {
  Database db;
  db.Put({ "id1", "Don't sell", "master", 1536107260, 1000 });
  db.Put({ "id2", "Rethink life", "master", 1536107260, 2000 });
  db.Put({ "id3", "Rethink life", "master2", 1536107260, 2000 });

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({ "id", "Have a hand", "not-master", 1536107260, 10 });
  db.Erase("id");
  db.Put({ "id", final_body, "not-master", 1536107260, -10 });

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}
