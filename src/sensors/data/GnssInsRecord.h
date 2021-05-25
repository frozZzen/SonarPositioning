#pragma once

#include "../../common/types.h"

#include <memory>
#include <ostream>

namespace sp::sensors::data
{
  using namespace types;

  struct GnssInsRecord
  {
    using Ptr = std::unique_ptr<GnssInsRecord>;

    Timepoint _timestamp;
    Radian    _roll;
    Radian    _pitch;
    Radian    _heading;
    Radian    _latitude;
    Radian    _longitude;
    Meter     _altitude;
    Meter     _heave;
  };
}

namespace std
{
  inline ostream& operator<<(ostream& stream_, const ::sp::sensors::data::GnssInsRecord& record_)
  {
    return stream_ << "GnssInsRecord{Roll: " << record_._roll << ", Pitch: " << record_._pitch <<
      ", Heading: " << record_._heading << ", Latitude: " << record_._latitude <<
      ", Longitude: " << record_._longitude << ", Altitude: " << record_._altitude <<
      ", Heave: " << record_._heave << '}';
  }
}
