#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

namespace Ini
{
  using Section = std::unordered_map<std::string, std::string>;
  
  class Document
  {
  public:
    Section& AddSection(std::string name);
    const Section& GetSection(const std::string& name) const;
    std::size_t SectionCount() const;
  
  private:
    std::unordered_map<std::string, Section> sections;
  };

  Document Load(std::istream& input);
}
