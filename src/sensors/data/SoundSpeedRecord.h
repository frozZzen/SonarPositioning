#pragma once

#include "../../common/types.h"

#include <ostream>

namespace sp::sensors::data
{
  using namespace types;

  struct SoundSpeedRecord
  {
    Timepoint       _timestamp;
    MeterPerSeccond _speed;
  };
}

namespace std
{
  static ostream& operator<<(ostream& stream_, const ::sp::sensors::data::SoundSpeedRecord& record_)
  {
    return stream_ << "Speed: " << record_._speed;
  }
}
