#pragma once

#include <deque>
#include <future>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <vector>
using namespace std;

class InvertedIndex
{
public:
  void Add(const string& document);
  std::vector<std::pair<std::size_t, std::size_t>> Lookup(const std::string& word) const;
private:
  std::size_t docid = 0;
  std::map<std::string, std::vector<std::pair<std::size_t, std::size_t>>> index;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex m_index;
  std::mutex m_sync_mutex;
  
  std::deque<std::future<void>> m_update_db_futs;
  const int MAX_UPDATE_DB_FUTS = 10;
  bool m_is_first_launch = true;

  std::deque<std::future<void>> m_add_queries_futs;
  const int MAX_ADD_QUERIES_FUTS = 10;
};
