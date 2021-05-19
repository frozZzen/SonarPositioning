#pragma once

#include <chrono>

namespace sp::types
{
  using Duration = std::chrono::microseconds;

  struct Clock : std::chrono::high_resolution_clock
  {
    using Timepoint = std::chrono::time_point<Clock, Duration>;
    static Timepoint now() { return Timepoint{std::chrono::duration_cast<Duration>(time_point::clock::now().time_since_epoch())}; };
  };

  using Timepoint = Clock::Timepoint;
  using SysClock = std::chrono::system_clock;

  //These could be Boost.Units
  using Radian = double;
  using Meter = double;
  using MeterPerSecond = double;
}
