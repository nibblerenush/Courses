#include "profile.h"


LogDuration::LogDuration(const std::string& msg):
  message(msg + ": "),
  start(std::chrono::steady_clock::now())
{}

LogDuration::~LogDuration()
{
  auto finish = std::chrono::steady_clock::now();
  auto dur = finish - start;
  std::cerr << message
    << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()
    << " ms" << std::endl;
}
