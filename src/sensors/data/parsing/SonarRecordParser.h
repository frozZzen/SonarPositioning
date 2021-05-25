#pragma once

#include "../SonarRecord.h"
#include "../../../tools/time_conversion.h"

#include <optional>
#include <istream>

namespace sp::sensors::data::parsing
{
  inline std::optional<SonarRecord::Ptr> parseSonarRecord(std::istream& stream_)
  {
    auto record = std::make_unique<SonarRecord>();
    record->_samples.reserve(SonarRecord::EXPECTED_SAMPLE_COUNT);

    double time;
    stream_ >> time;
    record->_samplingStartTime = tools::time_conversion::toTimepoint(time);
    while (!stream_.eof())
    {
      SonarRecord::Samples::value_type sample;
      stream_ >> sample._angle;
      stream_.ignore(1, ',');
      stream_ >> sample._sampleIndex;
      record->_samples.push_back(std::move(sample));
    }
    return record;
  }
}
