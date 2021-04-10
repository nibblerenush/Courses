#include "phone_book.h"
#include "contact.pb.h"

#include <algorithm>
#include <utility>

PhoneBook::PhoneBook(std::vector<Contact> contacts) : m_contacts(std::move(contacts)) {
  std::sort(m_contacts.begin(), m_contacts.end(), [](const Contact& lhs, const Contact& rhs) {
    return lhs.name < rhs.name;
  });
}

class CompStruct {
public:
  CompStruct(std::string_view str) : m_str(str)
  {}

  CompStruct(const Contact& contact) : m_str(contact.name)
  {}

  std::string_view Str() const {
    return m_str;
  }

private:
  std::string_view m_str;
};

IteratorRange<std::vector<Contact>::const_iterator> PhoneBook::FindByNamePrefix(std::string_view name_prefix) const {
  if (name_prefix.empty()) {
    return { m_contacts.cbegin(), m_contacts.cend() };
  }
  
  const std::size_t prefix_length = name_prefix.length();
  auto result = std::equal_range(m_contacts.cbegin(), m_contacts.cend(), name_prefix, [prefix_length](CompStruct lhs, CompStruct rhs) {
    std::string_view lhs_sv = lhs.Str().substr(0, prefix_length);
    std::string_view rhs_sv = rhs.Str().substr(0, prefix_length);
    return lhs_sv < rhs_sv;
  });
  return { result.first, result.second };
}

void PhoneBook::SaveTo(std::ostream& output) const {
  PhoneBookSerialize::ContactList pb_contact_list;
  for (const auto& contact : m_contacts) {
    PhoneBookSerialize::Contact pb_contact;
    pb_contact.set_name(contact.name);
    
    if (contact.birthday) {
      PhoneBookSerialize::Date pb_birthday;
      pb_birthday.set_day(contact.birthday.value().day);
      pb_birthday.set_month(contact.birthday.value().month);
      pb_birthday.set_year(contact.birthday.value().year);
      *pb_contact.mutable_birthday() = pb_birthday;
    }
    
    for (const auto& phone : contact.phones) {
      pb_contact.add_phone_number(phone);
    }
    
    *pb_contact_list.add_contact() = pb_contact;
  }
  pb_contact_list.SerializeToOstream(&output);
}

PhoneBook DeserializePhoneBook(std::istream& input) {
  PhoneBookSerialize::ContactList pb_contact_list;
  if (pb_contact_list.ParseFromIstream(&input)) {
    std::vector<Contact> contacts;
    for (const auto& pb_contact : pb_contact_list.contact()) {
      Contact contact;
      contact.name = pb_contact.name();

      if (pb_contact.has_birthday()) {
        const PhoneBookSerialize::Date& pb_birthday = pb_contact.birthday();
        Date date = {
          pb_birthday.year(),
          pb_birthday.month(),
          pb_birthday.day()
        };
        contact.birthday = std::move(date);
      }
      else {
        contact.birthday = std::nullopt;
      }

      for (const auto& phone_number : pb_contact.phone_number()) {
        contact.phones.push_back(phone_number);
      }

      contacts.push_back(std::move(contact));
    }
    return PhoneBook{ std::move(contacts) };
  }
  else {
    return PhoneBook{ std::vector<Contact>{} };
  }
}
