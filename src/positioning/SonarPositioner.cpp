#include "SonarPositioner.h"

#include "CoordinateCalculator.h"
#include "geographic_types.h"
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

  void SonarPositioner::setDataCallback(DataCallback&& callback_)
  {
    _callback = std::move(callback_);
  }

  void SonarPositioner::onGnssInsSensorData(GnssInsRecord record_)
  {
    logDebug(record_);
    _lastGnssRecord = std::move(record_);
  }
  
  void SonarPositioner::onSonarSensorData(SonarRecord record_)
  {
    logDebug(record_);
    if (!_isDataConsistent || !_callback) [[unlikely]]
    {
      _isDataConsistent = _lastGnssRecord && _currSoundSpeedRecord;
      return;
    }

    //TODO: interpolate sound speed based on the time parameter (will need data caching)
    auto soundSpeedGetter = [this](auto) {
      return _currSoundSpeedRecord.value()._speed;
    };

    const auto& gnss = _lastGnssRecord.value();
    const auto sonarPos = GeoPos{gnss._latitude, gnss._longitude, gnss._altitude};
    const auto sonarAngle = Angle3d{gnss._roll, gnss._pitch, gnss._heading};

    auto positions = calculateSonarSampleCoordinates(record_, sonarPos, sonarAngle, soundSpeedGetter);

    _callback(sonarPos, std::move(positions));
  }

  void SonarPositioner::onSoundSpeedSensorData(SoundSpeedRecord record_)
  {
    logDebug(record_);
    _currSoundSpeedRecord = std::move(record_);
  }
}