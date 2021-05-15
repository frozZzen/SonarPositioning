#pragma once

#include "../GnssInsRecord.h"

#include <optional>
#include <istream>

namespace sp::sensors::data::parsing
{
  static std::optional<GnssInsRecord> parseGnssInsRecord(std::istream& stream_)
  {
    GnssInsRecord record;
    stream_ >> record._roll >> record._pitch >> record._heading >> record._heading >> record._longitude >>
      record._altitude >> record._heave;
    return record;
  }
}
