#include "Common.h"

#include <algorithm>
#include <mutex>
#include <list>

using namespace std;

class LruCache : public ICache {
public:
  LruCache(shared_ptr<IBooksUnpacker> books_unpacker, const Settings& settings) :
    m_books_unpacker(move(books_unpacker)), m_settings(settings)
  {}

  BookPtr GetBook(const string& book_name) override
  {
    lock_guard<mutex> guard(m_mutex);
    
    auto iter = find_if(m_cached_books.begin(), m_cached_books.end(), [&book_name](BookPtr book) {
      return book->GetName() == book_name;
    });

    if (iter != m_cached_books.end()) {
      m_cached_books.splice(m_cached_books.begin(), m_cached_books, iter);
      return *iter;
    }
    
    BookPtr new_book = m_books_unpacker->UnpackBook(book_name);
    size_t new_book_size = new_book->GetContent().length();
    
    while (m_cache_size + new_book_size > m_settings.max_memory)
    {
      if (m_cache_size == 0) {
        return new_book;
      }

      BookPtr last_book = m_cached_books.back();
      m_cached_books.pop_back();
      size_t last_book_size = last_book->GetContent().length();
      if (last_book_size < m_cache_size) {
        m_cache_size -= last_book_size;
      }
      else {
        m_cache_size = 0;
      }
    }
    
    m_cached_books.push_front(new_book);
    m_cache_size += new_book_size;
    return new_book;
  }

private:
  shared_ptr<IBooksUnpacker> m_books_unpacker;
  Settings m_settings;

  list<BookPtr> m_cached_books;
  size_t m_cache_size = 0;

  mutable mutex m_mutex;
};

unique_ptr<ICache> MakeCache(shared_ptr<IBooksUnpacker> books_unpacker, const ICache::Settings& settings)
{
  return make_unique<LruCache>(move(books_unpacker), settings);
}
