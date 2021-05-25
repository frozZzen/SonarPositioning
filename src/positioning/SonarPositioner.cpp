#include "SonarPositioner.h"

#include "coord_calc.h"
#include "../common/log.h"

using namespace sp::tools::geographic;
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

  void SonarPositioner::onGnssInsSensorData(GnssInsRecord::Ptr record_)
  {
    logDebug(record_);
    _lastGnssRecord = std::move(record_);
  }
  
  void SonarPositioner::onSonarSensorData(SonarRecord::Ptr record_)
  {
    logDebug(record_);
    if (!_isDataConsistent || !_callback) [[unlikely]]
    {
      _isDataConsistent = _lastGnssRecord && _currSoundSpeedRecord;
      return;
    }

    //TODO: interpolate sound speed based on the time parameter (will need data caching)
    auto soundSpeedGetter = [this](auto) {
      return _currSoundSpeedRecord.value()->_speed;
    };

    const auto& gnss = *_lastGnssRecord.value();
    const auto sonarPos = GeoPos{gnss._latitude, gnss._longitude, gnss._altitude};

    //TODO: interpolate sonar position and angle based on the time parameter (will need data caching)
    auto sonarPosAngleGetter = [&](auto) {
      return std::make_pair(sonarPos, Angle3d{gnss._roll, gnss._pitch, gnss._heading});
    };
    

    auto positions = coord_calc::calculateSonarSampleCoordinates(*record_, sonarPosAngleGetter, soundSpeedGetter);

    _callback(sonarPos, std::move(positions));
  }

  void SonarPositioner::onSoundSpeedSensorData(SoundSpeedRecord::Ptr record_)
  {
    logDebug(record_);
    _currSoundSpeedRecord = std::move(record_);
  }
}
