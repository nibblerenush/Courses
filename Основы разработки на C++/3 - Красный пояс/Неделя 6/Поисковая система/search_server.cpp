#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <set>
#include <sstream>
#include <string_view>

std::vector<std::string> SplitIntoWords(const std::string& line)
{
  std::istringstream words_input(line);
  return
  {
    std::make_move_iterator(std::istream_iterator<std::string>(words_input)),
    std::make_move_iterator(std::istream_iterator<std::string>())
  };
}

void UpdateDocumentBaseThread(std::istream& document_input, InvertedIndex& index, std::mutex& sync_mutex)
{
  InvertedIndex new_index;

  for (std::string current_document; std::getline(document_input, current_document); )
  {
    new_index.Add(std::move(current_document));
  }

  {
    std::lock_guard<std::mutex> guard(sync_mutex);
    index = std::move(new_index);
  }
}

SearchServer::SearchServer(std::istream& document_input)
{
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(std::istream& document_input)
{
  if (m_is_first_launch)
  {
    m_is_first_launch = false;
    UpdateDocumentBaseThread(document_input, m_index, m_sync_mutex);
  }
  else
  {
    if (m_update_db_futs.size() >= MAX_UPDATE_DB_FUTS)
    {
      m_update_db_futs.front().get();
      m_update_db_futs.pop_front();
    }
    
    m_update_db_futs.push_back(std::async(UpdateDocumentBaseThread,
      std::ref(document_input),
      std::ref(m_index),
      std::ref(m_sync_mutex)));
  }
}

void AddQueriesStreamThread(std::istream& query_input, std::ostream& search_results_output, InvertedIndex& index, std::mutex& sync_mutex)
{
  const std::size_t docSize = 11000;

  std::vector<std::size_t> my_docid_count(docSize);
  std::vector<std::size_t> my_docid(docSize);

  std::vector<std::size_t>::iterator middle = docSize <= 5 ? my_docid.begin() + docSize : my_docid.begin() + 5;
  std::size_t outputCount = docSize <= 5 ? docSize : 5;

  for (std::string current_query; std::getline(query_input, current_query); )
  {
    for (std::size_t i = 0; i < docSize; ++i)
    {
      my_docid_count[i] = 0;
      my_docid[i] = i;
    }
    
    for (const auto& word : SplitIntoWords(current_query))
    {
      std::vector<std::pair<std::size_t, std::size_t>> index_lookup;
      {
        std::lock_guard<std::mutex> guard(sync_mutex);
        index_lookup = index.Lookup(word);
      }
      
      for (const auto& docid : index_lookup)
      {
        my_docid_count[docid.first] += docid.second;
      }
    }
    
    std::partial_sort(my_docid.begin(), middle, my_docid.begin() + docSize, [&my_docid_count](std::int64_t lhs, std::int64_t rhs)
    {
      return std::make_pair(my_docid_count[lhs], -lhs) > std::make_pair(my_docid_count[rhs], -rhs);
    });

    search_results_output << current_query << ':';
    for (std::size_t docid : Head(my_docid, outputCount))
    {
      const std::size_t hit_count = my_docid_count[docid];
      if (hit_count == 0) { break; }
      search_results_output << " {" << "docid: " << docid << ", " << "hitcount: " << hit_count << '}';
    }
    search_results_output << '\n';
  }
}

void SearchServer::AddQueriesStream(std::istream& query_input, std::ostream& search_results_output)
{
  if (m_add_queries_futs.size() >= MAX_ADD_QUERIES_FUTS)
  {
    m_add_queries_futs.front().get();
    m_add_queries_futs.pop_front();
  }

  m_add_queries_futs.push_back(std::async(AddQueriesStreamThread,
    std::ref(query_input),
    std::ref(search_results_output),
    std::ref(m_index),
    std::ref(m_sync_mutex)));
}

void InvertedIndex::Add(const std::string& document)
{
  std::set<std::string_view> checker;
  for (const auto& word : SplitIntoWords(document))
  {
    if (checker.insert(word).second)
    {
      index[word].emplace_back(docid, 1);
    }
    else
    {
      index[word].back().second++;
    }
  }

  docid++;
}

std::vector<std::pair<std::size_t, std::size_t>> InvertedIndex::Lookup(const std::string& word) const
{
  if (auto it = index.find(word); it != index.end())
  {
    return it->second;
  }
  else
  {
    return {};
  }
}
