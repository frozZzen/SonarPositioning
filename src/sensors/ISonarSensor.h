#pragma once

#include "data/SonarRecord.h"

#include <functional>

namespace sp::sensors
{
  class ISonarSensor
  {
  public:
    using Record        = data::SonarRecord;
    using DataCallback  = std::function<void (Record)>;

    virtual void setDataCallback(DataCallback&& callback_) = 0;

    virtual ~ISonarSensor() = default;
  };
}
