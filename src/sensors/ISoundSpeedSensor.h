#pragma once

#include "data/SoundSpeedRecord.h"

#include <functional>

namespace sp::sensors
{
  class ISoundSpeedSensor
  {
  public:
    using Record        = data::SoundSpeedRecord;
    using DataCallback  = std::function<void (Record)>;

    virtual void setDataCallback(DataCallback&& callback_) = 0;

    virtual ~ISoundSpeedSensor() = default;
  };
}
