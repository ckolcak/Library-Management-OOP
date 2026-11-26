#ifndef TIMEUTILS_H 
#define TIMEUTILS_H

#include <string>
#include <chrono>

using SysClock = std::chrono::system_clock;

std::string timeToString(SysClock::time_point timePoint);
SysClock::time_point stringToTime(const std::string& s);

#endif