#pragma once

#include "../SoundSpeedRecord.h"

#include <optional>
#include <istream>

namespace sp::sensors::data::parsing
{
  inline std::optional<SoundSpeedRecord::Ptr> parseSoundSpeedRecord(std::istream& stream_)
  {
    auto record = std::make_unique<SoundSpeedRecord>();
    stream_ >> record->_speed;
    return record;
  }
}
