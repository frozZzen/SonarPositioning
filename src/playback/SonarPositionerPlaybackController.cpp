#include "SonarPositionerPlaybackController.h"

namespace sp::playback
{
  SonarPositionerPlaybackController::SonarPositionerPlaybackController(Config& config_)
    : _gnssDevice(config_._gnssInsSensorPlaybackDeviceDataPath)
    , _sonarDevice(config_._sonarSensorPlaybackDeviceDataPath)
    , _soundDevice(config_._soundSpeedSensorPlaybackDeviceDataPath)
    , _executor(config_._executionTimeMultiplier)
  {
    using namespace sensors;

    auto& repo = config_._context._objectRepo;
    repo.set<IGnssInsSensor>([this]()     { return std::ref(_gnssDevice); });
    repo.set<ISonarSensor>([this]()       { return std::ref(_sonarDevice); });
    repo.set<ISoundSpeedSensor>([this]()  { return std::ref(_soundDevice); });

    _executor.registerExecutable(_gnssDevice);
    _executor.registerExecutable(_sonarDevice);
    _executor.registerExecutable(_soundDevice);
  }

  SonarPositionerPlaybackController::~SonarPositionerPlaybackController()
  {
    _executor.stop();
  }

  void SonarPositionerPlaybackController::start()
  {
    _executor.start();
  }

  void SonarPositionerPlaybackController::waitPlaybackAndStop()
  {
    _executor.waitExecutablesAndStop();
  }
}
