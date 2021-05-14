#pragma once

#include "../common/types.h"

#include <optional>

namespace sp::playback
{
  using namespace types;

  class IPlaybackDevice
  {
  public:
    virtual std::optional<Timepoint> onEvent(Timepoint currentTime_) = 0;
  };
}
