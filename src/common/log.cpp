#include "log.h"

#if LOG_LEVEL > 0
#include "types.h"
#include "../tools/time_conversion.h"

#include <ctime>
#include <filesystem>
#include <mutex>

using namespace sp::types;

namespace sp::log_details
{
  static std::mutex mutex;

  void logHeader(const char* severity_)
  {
    std::cout << severity_ << " [" << tools::toString(SysClock::now()) << "] - ";
  }

  void logTail(const char* file_, size_t line_)
  {
    std::wcout << "  (" << std::filesystem::path{file_}.filename().c_str();
    std::cout << ':' << line_ << ")";
  }

  void logLock()
  {
    mutex.lock();
  }

  void logUnlock()
  {
    mutex.unlock();
  }
}
#endif
