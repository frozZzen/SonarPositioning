#pragma once

#include "ISensor.h"
#include "data/GnssInsRecord.h"

namespace sp::sensors
{
  using IGnssInsSensor = ISensor<data::GnssInsRecord>;
}
