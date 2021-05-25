#pragma once

#include "../GnssInsRecord.h"

#include <optional>
#include <istream>

namespace sp::sensors::data::parsing
{
  inline std::optional<GnssInsRecord::Ptr> parseGnssInsRecord(std::istream& stream_)
  {
    auto record = std::make_unique<GnssInsRecord>();
    stream_ >> record->_roll >> record->_pitch >> record->_heading >> record->_latitude >> record->_longitude >>
      record->_altitude >> record->_heave;
    return record;
  }
}
