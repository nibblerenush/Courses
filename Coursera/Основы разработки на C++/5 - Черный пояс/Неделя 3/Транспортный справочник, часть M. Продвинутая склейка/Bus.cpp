#include "Bus.h"

#include <utility>

using namespace std;

Bus::Bus(string number) : m_number(move(number))
{}

void Bus::SetNumber(string number) {
  m_number = move(number);
}

const string& Bus::GetNumber() const {
  return m_number;
}

void Bus::SetRoute(Route route) {
  m_route = move(route);
}

const Route& Bus::GetRoute() const {
  return m_route;
}

Route& Bus::GetRoute() {
  return m_route;
}
