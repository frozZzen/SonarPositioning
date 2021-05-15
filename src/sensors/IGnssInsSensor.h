#pragma once

#include "data/GnssInsRecord.h"

#include <functional>

namespace sp::sensors
{
  class IGnssInsSensor
  {
  public:
    using Record        = data::GnssInsRecord;
    using DataCallback  = std::function<void (Record)>;

    virtual void setDataCallback(DataCallback&& callback_) = 0;

    virtual ~IGnssInsSensor() = default;
  };
}
