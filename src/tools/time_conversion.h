#pragma once

#include "../common/types.h"

#include <chrono>
#include <string>

namespace sp::tools
{
  using namespace types;

  template <typename FromTimestamp, typename ToTimestamp>
  void timepointCast(const FromTimestamp& from_, ToTimestamp& to_);

  Timepoint toTimepoint(double time_);

  std::string toString(SysClock::time_point time_);
  std::string toString(Timepoint time_);
}

#include <iostream>

template <typename FromTimestamp, typename ToTimestamp>
void sp::tools::timepointCast<FromTimestamp, ToTimestamp>(const FromTimestamp& from_, ToTimestamp& to_)
{
  const auto fromNow = typename FromTimestamp::clock::now();
  const auto toNow = typename ToTimestamp::clock::now();
  const auto fromNowEpoch = std::chrono::duration_cast<typename ToTimestamp::duration>(fromNow.time_since_epoch());
  const auto toNowEpoch = std::chrono::duration_cast<typename ToTimestamp::duration>(toNow.time_since_epoch());
  const auto fromEpoch = std::chrono::duration_cast<typename ToTimestamp::duration>(from_.time_since_epoch());
  to_ = ToTimestamp{toNowEpoch - fromNowEpoch + fromEpoch};
}
