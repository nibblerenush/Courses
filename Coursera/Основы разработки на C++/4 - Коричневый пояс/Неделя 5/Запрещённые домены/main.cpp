#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <utility>
#include <sstream>

using namespace std;

bool IsSubdomain(const string& subdomain, const string& domain) {
  auto i = static_cast<int>(subdomain.size()) - 1;
  auto j = static_cast<int>(domain.size()) - 1;
  
  while (i >= 0 && j >= 0) {
    if (subdomain[i] != domain[j]) {
      return false;
    }
    i--;
    j--;
  }
  
  if (i < 0 && j < 0) {
    return true;
  }
  else {
    return (i < 0 && domain[j] == '.') || (j < 0 && subdomain[i] == '.');
  }
}

vector<string> ReadDomains(istream& input_stream = cin) {
  size_t count;
  input_stream >> count;

  vector<string> domains;
  for (size_t i = 0; i < count; ++i) {
    string domain;
    input_stream >> domain;
    domains.push_back(move(domain));
  }
  return domains;
}

class DomainComp
{
public:
  bool operator () (const string& lhs, const string& rhs) const
  {
    auto i = static_cast<int>(lhs.size()) - 1;
    auto j = static_cast<int>(rhs.size()) - 1;
    while (i >= 0 && j >= 0 && lhs[i] == rhs[j]) {
      i--;
      j--;
    }
    
    if (i >= 0 && j >= 0) {
      return lhs[i] < rhs[j];
    }
    else if (j < 0) {
      return false;
    }
    else {
      return true;
    }
  }
};

int main()
{
  vector<string> banned_domains = ReadDomains();
  vector<string> domains_to_check = ReadDomains();
  sort(banned_domains.begin(), banned_domains.end(), DomainComp());
  
  size_t insert_pos = 0;
  for (string& domain : banned_domains) {
    if (insert_pos == 0 || !IsSubdomain(domain, banned_domains[insert_pos - 1])) {
      string temp_domain = move(domain);
      domain = move(banned_domains[insert_pos]);
      banned_domains[insert_pos++] = move(temp_domain);
    }
  }
  banned_domains.resize(insert_pos);
  
  for (const auto& domain : domains_to_check) {
    if (const auto it = upper_bound(begin(banned_domains), end(banned_domains), domain, DomainComp());
      it != begin(banned_domains) && IsSubdomain(domain, *prev(it))) {
      cout << "Bad" << endl;
    }
    else {
      cout << "Good" << endl;
    }
  }

  return 0;
}
