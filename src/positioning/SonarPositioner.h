#pragma once

#include "../common/Context.h"
#include "../playback/SonarPositionerPlaybackController.h"

#include <functional>
#include <vector>

namespace sp::positioning
{
  using namespace types;
  using namespace sensors;
  using namespace sensors::data;

  class SonarPositioner
  {
  public:
    SonarPositioner(Context& context_);

  private:
    void onGnssInsSensorData(GnssInsRecord record_);
    void onSonarSensorData(SonarRecord record_);
    void onSoundSpeedSensorData(SoundSpeedRecord record_);

    std::optional<GnssInsRecord>    _lastGnssRecord;
    std::optional<SoundSpeedRecord> _currSoundSpeedRecord;
    bool                            _isDataConsistent;
  };
}
