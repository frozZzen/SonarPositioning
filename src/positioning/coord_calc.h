#pragma once

#include "../common/types.h"
#include "../sensors/data/SonarRecord.h"
#include "../tools/geographic/types.h"

#include <functional>
#include <vector>

namespace sp::positioning::coord_calc
{
  using namespace sensors::data;
  using namespace tools::geographic;
  using namespace tools::math;
  using namespace types;

  using SoundSpeedGetter = std::function<MeterPerSecond (Timepoint)>;
  using SonarPosAngleGetter = std::function<std::pair<GeoPos, Angle3d> (Timepoint)>;

  std::vector<GeoPos> calculateSonarSampleCoordinates(
    const SonarRecord& record_, const SonarPosAngleGetter& getSonarPosAngle_, const SoundSpeedGetter& getSoundSpeed_);
}
