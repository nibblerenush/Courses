#pragma once

#include "http_request.h"

#include <map>
#include <set>
#include <string>
#include <string_view>

class Stats
{
public:
  Stats();
  void AddMethod(std::string_view method);
  void AddUri(std::string_view uri);
  const std::map<std::string_view, int>& GetMethodStats() const;
  const std::map<std::string_view, int>& GetUriStats() const;

private:
  static void AddStats(std::map<std::string_view, int>& stats, std::string_view known_key, std::string_view unknown_key);

private:
  std::map<std::string_view, int> m_method_stats;
  std::map<std::string_view, int> m_uri_stats;
};

HttpRequest ParseRequest(std::string_view line);
