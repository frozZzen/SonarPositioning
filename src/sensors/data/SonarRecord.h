#pragma once

#include "../../common/types.h"
#include "../../tools/time_conversion.h"

#include <ostream>
#include <vector>

namespace sp::sensors::data
{
  using namespace types;

  struct SonarSampleRecord
  {
    Radian    _angle;
    int       _sampleIndex;
  };

  struct SonarRecord
  {
    using Samples = std::vector<SonarSampleRecord>;

    static constexpr size_t EXPECTED_SAMPLE_COUNT = 512;

    int       _samplingRate = 78'125;
    Timepoint _timestamp;
    Samples   _samples;
  };
}

namespace std
{
  inline ostream& operator<<(ostream& stream_, const ::sp::sensors::data::SonarSampleRecord& record_)
  {
    return stream_ << "Angle: " << record_._angle << ", SampleIndex: " << record_._sampleIndex;
  }

  inline ostream& operator<<(ostream& stream_, const ::sp::sensors::data::SonarRecord& record_)
  {
    using namespace sp::tools;
    stream_ << "SonarSampleRecord{Timestamp: " << toString(record_._timestamp) << ", Samples: [";
    for (size_t i = 0; i < record_._samples.size() - 1; ++i)
    {
      const auto& sample = record_._samples[i];
      stream_ << '(' << sample << "), ";
    }
    if (!record_._samples.empty())
    {
      stream_ << '(' << record_._samples.back() << ')';
    }
    return stream_ << ']}';
  }
}
