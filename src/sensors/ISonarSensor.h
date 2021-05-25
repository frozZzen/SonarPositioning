#pragma once

#include "ISensor.h"
#include "data/SonarRecord.h"

namespace sp::sensors
{
  using ISonarSensor = ISensor<data::SonarRecord>;
}
