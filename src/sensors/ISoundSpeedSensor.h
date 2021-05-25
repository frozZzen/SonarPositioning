#pragma once

#include "ISensor.h"
#include "data/SoundSpeedRecord.h"

namespace sp::sensors
{
  using ISoundSpeedSensor = ISensor<data::SoundSpeedRecord>;
}
