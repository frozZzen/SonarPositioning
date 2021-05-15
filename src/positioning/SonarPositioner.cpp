#include "SonarPositioner.h"

#include "../common/log.h"

using namespace std::chrono_literals;

namespace sp::positioning
{
  SonarPositioner::SonarPositioner(Context& context_)
    : _isDataConsistent(false)
  {
    context_._objectRepo.get<sensors::IGnssInsSensor>()().get().setDataCallback(
      [this](auto record_) { onGnssInsSensorData(std::move(record_)); });
    context_._objectRepo.get<sensors::ISonarSensor>()().get().setDataCallback(
      [this](auto record_) { onSonarSensorData(std::move(record_)); });
    context_._objectRepo.get<sensors::ISoundSpeedSensor>()().get().setDataCallback(
      [this](auto record_) { onSoundSpeedSensorData(std::move(record_)); });
  }

  void SonarPositioner::onGnssInsSensorData(GnssInsRecord record_)
  {
    logDebug("GnssInsRecord: ", record_);
    _lastGnssRecord = std::move(record_);
  }
  
  void SonarPositioner::onSonarSensorData(SonarRecord record_)
  {
    logDebug("SonarRecord: ", record_);
    if (!_isDataConsistent) [[unlikely]]
    {
      _isDataConsistent = _lastGnssRecord && _currSoundSpeedRecord;
      return;
    }
    for (const auto& sample : record_._samples)
    {
      auto elapsedTime = 1.0 * sample._sampleIndex / record_._samplingRate;
      auto traveledDistance = _currSoundSpeedRecord.value()._speed * elapsedTime;
      logInfo("traveledDistance: ", traveledDistance);
    }
  }

  void SonarPositioner::onSoundSpeedSensorData(SoundSpeedRecord record_)
  {
    logInfo("SoundSpeedRecord: ", record_);
    _currSoundSpeedRecord = std::move(record_);
  }
}
