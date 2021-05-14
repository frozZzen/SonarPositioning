#pragma once

#include <chrono>

namespace sp::types
{
  using Clock = std::chrono::high_resolution_clock;
  using Duration = std::chrono::nanoseconds;
  using Timepoint = std::chrono::time_point<Clock>;
}
