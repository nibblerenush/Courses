#include "ini.h"
#include <utility>

namespace Ini
{
  Section& Document::AddSection(std::string name)
  {
    return sections.emplace(std::move(name), Section{}).first->second;
  }

  const Section& Document::GetSection(const std::string& name) const
  {
    return sections.at(name);
  }

  std::size_t Document::SectionCount() const
  {
    return sections.size();
  }
}

Ini::Document Ini::Load(std::istream& input)
{
  Ini::Document doc;
  Ini::Section* cur_section = nullptr;
  for (std::string line; std::getline(input, line); )
  {
    if (!line.empty())
    {
      // Section
      if (line[0] == '[')
      {
        cur_section = &doc.AddSection(line.substr(1, line.length() - 2));
      }
      // key=value
      else
      {
        std::size_t equal_sign = line.find('=');
        cur_section->emplace(line.substr(0, equal_sign), line.substr(equal_sign + 1));
      }
    }
  }
  return doc;
}
