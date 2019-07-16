#pragma once

#include <ctime>
#include <string>

static std::string GetCurrentDate() {
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    return std::to_string(now->tm_year + 1900) + '-' 
         + std::to_string(now->tm_mon + 1) + '-'
         +  std::to_string(now->tm_mday)
         + "\n";
}