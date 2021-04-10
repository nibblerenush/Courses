#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

template <typename It>
class Range {
public:
  Range(It begin, It end) : begin_(begin), end_(end) {}
  It begin() const { return begin_; }
  It end() const { return end_; }

private:
  It begin_;
  It end_;
};

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter = " ") {
  const size_t pos = s.find(delimiter);
  if (pos == s.npos) {
    return { s, nullopt };
  }
  else {
    return { s.substr(0, pos), s.substr(pos + delimiter.length()) };
  }
}

vector<string_view> Split(string_view s, string_view delimiter = " ") {
  vector<string_view> parts;
  if (s.empty()) {
    return parts;
  }
  while (true) {
    const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
    parts.push_back(lhs);
    if (!rhs_opt) {
      break;
    }
    s = *rhs_opt;
  }
  return parts;
}


class Domain {
public:
  explicit Domain(string_view text) {
    vector<string_view> parts = Split(text, ".");
    parts_reversed_.assign(rbegin(parts), rend(parts));
  }

  size_t GetPartCount() const {
    return parts_reversed_.size();
  }

  auto GetParts() const {
    return Range(rbegin(parts_reversed_), rend(parts_reversed_));
  }
  auto GetReversedParts() const {
    return Range(begin(parts_reversed_), end(parts_reversed_));
  }

  bool operator==(const Domain& other) const {
    return parts_reversed_ == other.parts_reversed_;
  }

private:
  vector<string> parts_reversed_;
};

ostream& operator<<(ostream& stream, const Domain& domain) {
  bool first = true;
  for (const string_view part : domain.GetParts()) {
    if (!first) {
      stream << '.';
    }
    else {
      first = false;
    }
    stream << part;
  }
  return stream;
}

// domain is subdomain of itself
bool IsSubdomain(const Domain& subdomain, const Domain& domain) {
  const auto subdomain_reversed_parts = subdomain.GetReversedParts();
  const auto domain_reversed_parts = domain.GetReversedParts();
  return
    subdomain.GetPartCount() >= domain.GetPartCount()
    && equal(begin(domain_reversed_parts), end(domain_reversed_parts),
      begin(subdomain_reversed_parts));
}

bool IsSubOrSuperDomain(const Domain& lhs, const Domain& rhs) {
  return lhs.GetPartCount() >= rhs.GetPartCount()
    ? IsSubdomain(lhs, rhs)
    : IsSubdomain(rhs, lhs);
}


class DomainChecker {
public:
  template <typename InputIt>
  DomainChecker(InputIt domains_begin, InputIt domains_end) {
    sorted_domains_.reserve(distance(domains_begin, domains_end));
    for (const Domain& domain : Range(domains_begin, domains_end)) {
      sorted_domains_.push_back(&domain);
    }
    sort(begin(sorted_domains_), end(sorted_domains_), IsDomainLess);
    sorted_domains_ = AbsorbSubdomains(move(sorted_domains_));
  }

  // Check if candidate is subdomain of some domain
  bool IsSubdomain(const Domain& candidate) const {
    const auto it = upper_bound(
      begin(sorted_domains_), end(sorted_domains_),
      &candidate, IsDomainLess);
    if (it == begin(sorted_domains_)) {
      return false;
    }
    return ::IsSubdomain(candidate, **prev(it));
  }

private:
  vector<const Domain*> sorted_domains_;

  static bool IsDomainLess(const Domain* lhs, const Domain* rhs) {
    const auto lhs_reversed_parts = lhs->GetReversedParts();
    const auto rhs_reversed_parts = rhs->GetReversedParts();
    return lexicographical_compare(
      begin(lhs_reversed_parts), end(lhs_reversed_parts),
      begin(rhs_reversed_parts), end(rhs_reversed_parts)
      );
  }

  static vector<const Domain*> AbsorbSubdomains(vector<const Domain*> domains) {
    domains.erase(
      unique(begin(domains), end(domains),
        [](const Domain* lhs, const Domain* rhs) {
      return IsSubOrSuperDomain(*lhs, *rhs);
    }),
      end(domains)
      );
    return domains;
  }
};


vector<Domain> ReadDomains(istream& in_stream = cin) {
  vector<Domain> domains;

  size_t count;
  in_stream >> count;
  domains.reserve(count);

  for (size_t i = 0; i < count; ++i) {
    string domain_text;
    in_stream >> domain_text;
    domains.emplace_back(domain_text);
  }
  return domains;
}

vector<bool> CheckDomains(const vector<Domain>& banned_domains, const vector<Domain>& domains_to_check) {
  const DomainChecker checker(begin(banned_domains), end(banned_domains));

  vector<bool> check_results;
  check_results.reserve(domains_to_check.size());
  for (const Domain& domain_to_check : domains_to_check) {
    check_results.push_back(!checker.IsSubdomain(domain_to_check));
  }

  return check_results;
}

void PrintCheckResults(const vector<bool>& check_results, ostream& out_stream = cout) {
  for (const bool check_result : check_results) {
    out_stream << (check_result ? "Good" : "Bad") << "\n";
  }
}

void TestBug1() {
  string domain = "m.ya.ru";
  vector<string_view> result = Split(domain, ".");
  ASSERT_EQUAL(result[0], "m");
  ASSERT_EQUAL(result[1], "ya");
  ASSERT_EQUAL(result[2], "ru");
}

void TestBug2() {
  Domain domain("m.ya.ru");
  auto range = domain.GetReversedParts();
  vector<string> check(range.begin(), range.end());
  vector<string> expected = { "ru", "ya", "m" };
  ASSERT_EQUAL(check, expected);
}

void TestBug3() {
  Domain subdomain("ya.ru");
  Domain domain("ya.ru");
  ASSERT(IsSubdomain(subdomain, domain));
  ASSERT(IsSubdomain(domain, subdomain));
}

void TestBug4() {
  {
    Domain subdomain("maps.ya.ru");
    Domain domain("ya.ru");
    ASSERT(IsSubdomain(subdomain, domain));
    ASSERT(!IsSubdomain(domain, subdomain));
  }
  
  {
    Domain subdomain("mail.ru");
    Domain domain("yandex.ru");
    ASSERT(!IsSubdomain(subdomain, domain));
    ASSERT(!IsSubdomain(domain, subdomain));
  }
}

void TestBug5() {
  {
    Domain subdomain("maps.ya.ru");
    Domain domain("ya.ru");
    ASSERT(IsSubOrSuperDomain(subdomain, domain));
    ASSERT(IsSubOrSuperDomain(domain, subdomain));
  }
  
  {
    Domain subdomain("ya.ru");
    Domain domain("ya.ru");
    ASSERT(IsSubOrSuperDomain(subdomain, domain));
    ASSERT(IsSubOrSuperDomain(domain, subdomain));
  }

  {
    Domain subdomain("mail.ru");
    Domain domain("yandex.ru");
    ASSERT(!IsSubOrSuperDomain(subdomain, domain));
    ASSERT(!IsSubOrSuperDomain(domain, subdomain));
  }


}

void TestBug6() {
  vector<Domain> domains;
  domains.push_back(Domain("abba.ya.ru"));
  domains.push_back(Domain("ya.ru"));
  domains.push_back(Domain("sber.ru"));
  DomainChecker domain_checker(domains.begin(), domains.end());

  ASSERT(domain_checker.IsSubdomain(Domain("ya.ru")));
  ASSERT(domain_checker.IsSubdomain(Domain("maps.ya.ru")));
  ASSERT(domain_checker.IsSubdomain(Domain("sber.ru")));

  ASSERT(domain_checker.IsSubdomain(Domain("m.ya.ru")));
  ASSERT(domain_checker.IsSubdomain(Domain("m.maps.ya.ru")));
  ASSERT(domain_checker.IsSubdomain(Domain("m.sber.ru")));

  ASSERT(domain_checker.IsSubdomain(Domain("n.m.ya.ru")));
  ASSERT(domain_checker.IsSubdomain(Domain("n.m.maps.ya.ru")));
  ASSERT(domain_checker.IsSubdomain(Domain("n.m.sber.ru")));

  ASSERT(!domain_checker.IsSubdomain(Domain("tasks.ru")));
  ASSERT(!domain_checker.IsSubdomain(Domain("ru")));
}

void TestBug7() {
  vector<Domain> banned_domains;
  banned_domains.push_back(Domain("ya.ru"));
  banned_domains.push_back(Domain("maps.ya.ru"));
  banned_domains.push_back(Domain("com"));
  
  vector<Domain> check_domains;
  check_domains.push_back(Domain("ya.ru"));
  check_domains.push_back(Domain("maps.ya.ru"));
  check_domains.push_back(Domain("mail.ru"));
  check_domains.push_back(Domain("mail.sber.ru"));
  check_domains.push_back(Domain("dev.com"));

  ostringstream oss;
  PrintCheckResults(CheckDomains(banned_domains, check_domains), oss);
  ASSERT_EQUAL(oss.str(), "Bad\nBad\nGood\nGood\nBad\n");
}

void TestBug8() {
  stringstream ss;
  ss << 2 << '\n' << "yandex.ru\n" << "mail.ru\n";
  const vector<Domain> domains = ReadDomains(ss);
  ASSERT_EQUAL(domains.size(), 2);

  {
    auto range = domains[0].GetParts();
    vector<string> check(range.begin(), range.end());
    vector<string> expected = { "yandex", "ru"};
    ASSERT_EQUAL(check, expected);
  }
  ASSERT_EQUAL(domains[0].GetPartCount(), 2);

  {
    auto range = domains[1].GetParts();
    vector<string> check(range.begin(), range.end());
    vector<string> expected = { "mail", "ru" };
    ASSERT_EQUAL(check, expected);
  }
  ASSERT_EQUAL(domains[1].GetPartCount(), 2);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBug1);
  RUN_TEST(tr, TestBug2);
  RUN_TEST(tr, TestBug3);
  RUN_TEST(tr, TestBug4);
  RUN_TEST(tr, TestBug5);
  RUN_TEST(tr, TestBug6);
  RUN_TEST(tr, TestBug7);
  RUN_TEST(tr, TestBug8);

  /*const vector<Domain> banned_domains = ReadDomains();
  const vector<Domain> domains_to_check = ReadDomains();
  PrintCheckResults(CheckDomains(banned_domains, domains_to_check));*/
  return 0;
}
