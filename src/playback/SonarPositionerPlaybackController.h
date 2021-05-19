#pragma once

#include "devices/GnssInsSensorPlaybackDevice.h"
#include "devices/SonarSensorPlaybackDevice.h"
#include "devices/SoundSpeedSensorPlaybackDevice.h"
#include "../common/Context.h"
#include "../tools/scheduling/TimedExecutor.h"

#include <functional>
#include <memory>
#include <vector>

namespace sp::playback
{
  using namespace devices;
  using namespace types;

  class SonarPositionerPlaybackController
  {
  public:
    struct Config
    {
      Context&              _context;
      std::filesystem::path _gnssInsSensorPlaybackDeviceDataPath;
      std::filesystem::path _sonarSensorPlaybackDeviceDataPath;
      std::filesystem::path _soundSpeedSensorPlaybackDeviceDataPath;
      double                _executionTimeMultiplier;
    };

    SonarPositionerPlaybackController(Config& config_);
    ~SonarPositionerPlaybackController();

    void start();
    void waitPlaybackAndStop();

  private:
    GnssInsSensorPlaybackDevice     _gnssDevice;
    SonarSensorPlaybackDevice       _sonarDevice;
    SoundSpeedSensorPlaybackDevice  _soundDevice;
    scheduling::TimedExecutor       _executor;
  };
}