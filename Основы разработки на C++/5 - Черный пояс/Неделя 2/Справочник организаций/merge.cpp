#include "yellow_pages.h"

#include <algorithm>
#include <tuple>

namespace YellowPages {

  bool operator < (const Name& lhs, const Name& rhs) {
    Name::Type lhs_type = lhs.type();
    Name::Type rhs_type = rhs.type();
    return std::tie(lhs.value(), lhs_type) < std::tie(rhs.value(), rhs_type);
  }

  bool operator < (const Phone& lhs, const Phone& rhs) {
    Phone::Type lhs_type = lhs.type();
    Phone::Type rhs_type = rhs.type();
    return
      std::tie(lhs.formatted(), lhs_type, lhs.country_code(), lhs.local_code(), lhs.number(), lhs.extension(), lhs.description()) <
      std::tie(rhs.formatted(), rhs_type, rhs.country_code(), rhs.local_code(), rhs.number(), rhs.extension(), rhs.description());
  }

  bool operator < (const Url& lhs, const Url& rhs) {
    return lhs.value() < rhs.value();
  }

  Company Merge(const Signals& signals, const Providers& providers) {
    std::map<std::uint32_t, const Address*, std::greater<std::uint32_t>> priority_address;
    std::map<std::uint32_t, std::set<Name>, std::greater<std::uint32_t>> priority_name;
    std::map<std::uint32_t, std::set<Phone>, std::greater<std::uint32_t>> priority_phone;
    std::map<std::uint32_t, std::set<Url>, std::greater<std::uint32_t>> priority_url;
    std::map<std::uint32_t, const WorkingTime*, std::greater<std::uint32_t>> priority_working_time;

    for (const auto& signal : signals) {
      std::uint64_t provider_id = signal.provider_id();
      std::uint32_t provider_priority = providers.at(provider_id).priority();

      const Company& company = signal.company();

      if (company.has_address()) {
        priority_address[provider_priority] = &company.address();
      }

      if (company.names_size() != 0) {
        for (const auto& name : company.names()) {
          priority_name[provider_priority].insert(name);
        }
      }

      if (company.phones_size() != 0) {
        for (const auto& phone : company.phones()) {
          priority_phone[provider_priority].insert(phone);
        }
      }

      if (company.urls_size() != 0) {
        for (const auto& url : company.urls()) {
          priority_url[provider_priority].insert(url);
        }
      }

      if (company.has_working_time()) {
        priority_working_time[provider_priority] = &company.working_time();
      }
    }

    Company result;
    if (!priority_address.empty()) {
      *result.mutable_address() = *(priority_address.begin()->second);
    }

    if (!priority_name.empty()) {
      for (const Name& name : priority_name.begin()->second) {
        *result.add_names() = name;
      }
    }

    if (!priority_phone.empty()) {
      for (const Phone& phone : priority_phone.begin()->second) {
        *result.add_phones() = phone;
      }
    }

    if (!priority_url.empty()) {
      for (const Url& url : priority_url.begin()->second) {
        *result.add_urls() = url;
      }
    }

    if (!priority_working_time.empty()) {
      *result.mutable_working_time() = *(priority_working_time.begin()->second);
    }

    return result;
  }

}
