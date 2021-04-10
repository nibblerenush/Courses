#include "stats.h"
#include <array>

static const std::string UNKNOWN_METHOD = "UNKNOWN";
static const std::string UNKNOWN_URI = "unknown";

static const std::set<std::string> METHODS = { "GET", "POST", "PUT", "DELETE", UNKNOWN_METHOD };
static const std::set<std::string> URIS = { "/", "/order", "/product", "/basket", "/help", UNKNOWN_URI };

Stats::Stats()
{
  for (const auto& method : METHODS)
  {
    m_method_stats[method] = 0;
  }

  for (const auto& uri : URIS)
  {
    m_uri_stats[uri] = 0;
  }
}

void Stats::AddMethod(std::string_view method)
{
  AddStats(m_method_stats, method, UNKNOWN_METHOD);
}

void Stats::AddUri(std::string_view uri)
{
  AddStats(m_uri_stats, uri, UNKNOWN_URI);
}

const std::map<std::string_view, int>& Stats::GetMethodStats() const
{
  return m_method_stats;
}

const std::map<std::string_view, int>& Stats::GetUriStats() const
{
  return m_uri_stats;
}

void Stats::AddStats(std::map<std::string_view, int>& stats, std::string_view known_key, std::string_view unknown_key)
{
  auto iter = stats.find(known_key);
  if (iter != stats.end())
  {
    stats[known_key]++;
  }
  else
  {
    stats[unknown_key]++;
  }
}

HttpRequest ParseRequest(std::string_view line)
{
  std::size_t not_space = line.find_first_not_of(' ');
  line.remove_prefix(not_space);

  std::array<std::string_view, 3> parseResult;
  for (int i = 0; i < 3; ++i)
  {
    std::size_t space = line.find_first_of(' ');
    parseResult[i] = line.substr(0, space);
    line.remove_prefix(space + 1);
  }

  return { parseResult[0], parseResult[1], parseResult[2] };
}
