#pragma once

#include "../../common/types.h"

#include <ostream>

namespace sp::sensors::data
{
  using namespace types;

  struct SoundSpeedRecord
  {
    Timepoint       _timestamp;
    MeterPerSecond  _speed;
  };
}

namespace std
{
  inline ostream& operator<<(ostream& stream_, const ::sp::sensors::data::SoundSpeedRecord& record_)
  {
    return stream_ << "SoundSpeedRecord{Speed: " << record_._speed << '}';
  }
}
