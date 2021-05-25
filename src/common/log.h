#pragma once

#define ERROR_LOG_LEVEL 1
#define INFO_LOG_LEVEL 2
#define DEBUG_LOG_LEVEL 3

#define LOG_LEVEL INFO_LOG_LEVEL

#if LOG_LEVEL > 0
#define logError(...) sp::log_details::log("[ERROR]", 1, __FILE__, __LINE__, __VA_ARGS__)
#if LOG_LEVEL > 1
#define logInfo(...)  sp::log_details::log("[INFO] ", 2, __FILE__, __LINE__, __VA_ARGS__)
#if LOG_LEVEL > 2
#define logDebug(...) sp::log_details::log("[DEBUG]", 3, __FILE__, __LINE__, __VA_ARGS__)
#else
#define logDebug(...)
#endif
#else
#define logInfo(...)
#define logDebug(...)
#endif
#include <iostream>

namespace sp::log_details
{
  void logHeader(const char* severity_);
  void logTail(const char* file_, size_t line_);
  void logLock();
  void logUnlock();

  template <typename... Args>
  constexpr void log(const char* severity_, int level_, const char* file_, size_t line_, Args... args_)
  {
    logLock();
    logHeader(severity_);
    (std::cout << ... << (args_));
    logTail(file_, line_);
    std::cout << std::endl;
    logUnlock();
  }
}
#else
#define logError(...)
#define logInfo(...)
#define logDebug(...)
#endif
