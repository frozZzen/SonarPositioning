#pragma once

#include "../SoundSpeedRecord.h"

#include <optional>
#include <istream>

namespace sp::sensors::data::parsing
{
  inline std::optional<SoundSpeedRecord> parseSoundSpeedRecord(std::istream& stream_)
  {
    SoundSpeedRecord record;
    stream_ >> record._speed;
    return record;
  }
}
