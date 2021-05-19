#pragma once

#include "geographic_types.h"
#include "../common/types.h"
#include "../sensors/data/SonarRecord.h"

#include <functional>
#include <vector>

namespace sp::positioning
{
  using namespace sensors::data;
  using namespace tools::math;
  using namespace types;

  using SoundSpeedGetter = std::function<MeterPerSecond(Timepoint)>;

  std::vector<GeoPos> calculateSonarSampleCoordinates(
    const SonarRecord& record_, const GeoPos& sonarPos_, const Angle3d& sonarAngle_, const SoundSpeedGetter& getSoundSpeed_);

  CartPos calculateSamplePosRelativeToSonar(
    const SonarRecord& record_, const SonarSampleRecord& sample_, const SoundSpeedGetter& getSoundSpeed_);

  CartPos calculateGlobalSamplePosition(const CartPos& sonarPos_, const CartPos& relativePos_, const Angle3d& sonarAngle_);
}
