#include "responses.h"

std::string PopNewLine(std::string str)
{
  if (str.back() == '\n')
  {
    str.pop_back();
  }
  return str;
}

std::ostream& operator << (std::ostream& os, const BusesForStopResponse& r)
{
  return os << PopNewLine(r.response);
}

std::ostream& operator << (std::ostream& os, const StopsForBusResponse& r)
{
  return os << PopNewLine(r.response);
}

std::ostream& operator << (std::ostream& os, const AllBusesResponse& r)
{
  return os << PopNewLine(r.response);
}
