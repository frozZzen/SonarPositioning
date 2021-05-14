#pragma once

#include "../IPlaybackDevice.h"

namespace sp::playback
{
  using namespace types;

  class SoundSpeedSensorPlaybackDevice : public IPlaybackDevice
  {
  public:
    virtual std::optional<Timepoint> onEvent() override;
  };
}