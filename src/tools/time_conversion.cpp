#include "time_conversion.h"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace sp::tools::time_conversion
{
  Timepoint toTimepoint(double time_)
  {
    Timepoint result;
    int64_t seconds = static_cast<int64_t>(time_);
    const auto sysTime = SysClock::time_point{std::chrono::seconds{seconds}};
    tools::time_conversion::timepointCast(sysTime, result);
    return result + Duration{static_cast<int64_t>((time_ - seconds) * Duration::period::den)};
  }

  Duration toDuration(double seconds_)
  {
    return Duration{static_cast<int64_t>(seconds_ * Duration::period::den)};
  }

  std::string toString(SysClock::time_point time_)
  {
    const auto timeT = SysClock::to_time_t(time_);
    tm gmNow;
    gmtime_s(&gmNow, &timeT);
    const auto micro = std::chrono::duration_cast<SysClock::duration>(time_ - std::chrono::floor<std::chrono::seconds>(time_));
    std::stringstream timeStream;
    timeStream << std::put_time(&gmNow, "%Y%m%d-%H:%M:%S.") << std::right << std::setw(6) << std::setfill('0') << micro.count();
    return timeStream.str();
  }

  std::string toString(Timepoint time_)
  {
    SysClock::time_point convTime;
    timepointCast(time_, convTime);
    return toString(convTime);
  }
}
