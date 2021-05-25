#include "coord_calc.h"

#include "../common/log.h"

#include "GeographicLib/LocalCartesian.hpp"

using namespace sp::sensors::data;

namespace sp::positioning::coord_calc
{
  CartPos calculateSamplePosRelativeToSonar(
    const SonarRecord& record_, const SonarSampleRecord& sample_, const SoundSpeedGetter& getSoundSpeed_)
  {
    const auto elapsedTimeInSeconds = 1.0 * sample_._sampleIndex / record_._samplingRate;
    const auto elapsedDuration = tools::time_conversion::toDuration(elapsedTimeInSeconds);

    const auto soundSpeed = getSoundSpeed_(record_._samplingStartTime + elapsedDuration);
    const auto traveledDistance = soundSpeed * elapsedTimeInSeconds;
    logDebug("traveledDistance: ", traveledDistance, ", angle: ", sample_._angle,
      ", elapsedTimeInSec: ", elapsedTimeInSeconds, "soundSpeed: ", soundSpeed);

    Meter distanceToBed = traveledDistance / 2;
    Meter projY = -distanceToBed * sin(sample_._angle);
    Meter projZ = -distanceToBed * cos(sample_._angle);

    return {0, projY, projZ};
  }

  CartPos calculateGlobalSamplePosition(
    const CartPos& sonarPos_, const CartPos& relativePos_, const Angle3d& sonarAngle_)
  {
    auto samplePosWithSonarAngle = Vector3d<Meter>{relativePos_} | sonarAngle_;
    auto globalSamplePos = Vector3d<Meter>{sonarPos_} + samplePosWithSonarAngle;
    logDebug("Sample position: ", globalSamplePos);

    return globalSamplePos;
  }

  std::vector<GeoPos> calculateSonarSampleCoordinates(
    const SonarRecord& record_, const SonarPosAngleGetter& getSonarPosAngle_, const SoundSpeedGetter& getSoundSpeed_)
  {
    const auto& [sonarPos, sonarAngle] = getSonarPosAngle_(record_._samplingStartTime);

    CartPos refPos;
    GeographicLib::LocalCartesian cart;
    cart.Forward(sonarPos._lat, sonarPos._lon, sonarPos._alt, refPos._x, refPos._y, refPos._z);
    logDebug("Geodetic to local cartesian: ", pos_, " => ", refPos);

    std::vector<GeoPos> samplePositions;
    samplePositions.reserve(SonarRecord::EXPECTED_SAMPLE_COUNT);

    for (const auto& sample : record_._samples)
    {
      const auto relativePos = calculateSamplePosRelativeToSonar(record_, sample, getSoundSpeed_);
      const auto realPos = calculateGlobalSamplePosition(refPos, relativePos, sonarAngle);
      logDebug("Relative pos to global pos: ", relativePos, " => ", realPos);

      GeoPos realGeoPos;
      cart.Reverse(realPos._x, realPos._y, realPos._z, realGeoPos._lat, realGeoPos._lon, realGeoPos._alt);
      logDebug("Transformed local cartesian to geodetic: ", realPos, " => ", realGeoPos);

      samplePositions.push_back(realGeoPos);
    }

    return samplePositions;
  }
}
