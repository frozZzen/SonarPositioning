#pragma once

#include "../common/Context.h"
#include "../playback/SonarPositionerPlaybackController.h"
#include "../tools/geographic/types.h"

#include <functional>
#include <vector>

namespace sp::positioning
{
  using namespace types;
  using namespace sensors;
  using namespace sensors::data;
  using namespace tools::geographic;

  class SonarPositioner
  {
  public:
    using DataCallback = std::function<void (const GeoPos&, std::vector<GeoPos>)>;

    SonarPositioner(Context& context_);

    void setDataCallback(DataCallback&& callback_);

  private:
    void onGnssInsSensorData(GnssInsRecord record_);
    void onSonarSensorData(SonarRecord record_);
    void onSoundSpeedSensorData(SoundSpeedRecord record_);

    DataCallback                    _callback;
    std::optional<GnssInsRecord>    _lastGnssRecord;
    std::optional<SoundSpeedRecord> _currSoundSpeedRecord;
    bool                            _isDataConsistent;
  };
}
