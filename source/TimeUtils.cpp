#include <ctime>
#include <sstream>
#include <iomanip>
#include "TimeUtils.h"

SysClock::time_point stringToTime(const std::string& s) {
    std::tm tm = {};
    std::stringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    auto tp = SysClock::from_time_t(std::mktime(&tm));
    return tp;
}

std::string timeToString(SysClock::time_point timePoint) {
    std::time_t t = SysClock::to_time_t(timePoint); 
    std::tm tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

