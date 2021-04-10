#pragma once

#include "Route.h"

#include <string>

class Bus {
public:
  Bus(std::string number);
  void SetNumber(std::string number);
  const std::string& GetNumber() const;
  void SetRoute(Route route);
  const Route& GetRoute() const;
  Route& GetRoute();

private:
  std::string m_number;
  Route m_route;
};
