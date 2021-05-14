#pragma once

#include "IPlaybackDevice.h"

namespace sp::playback
{
  using namespace types;

  class PlaybackOrchestrator
  {
  public:
    IPlaybackDevice& registerDevice(IPlaybackDevice&& device_, std::optional<Timepoint> startTime_);
  };
}