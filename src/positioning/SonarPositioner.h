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
    using GnssInsRecordPtrOpt = std::optional<GnssInsRecord::Ptr>;
    using SoundSpeedRecordPtrOpt = std::optional<SoundSpeedRecord::Ptr>;


    void onGnssInsSensorData(GnssInsRecord::Ptr record_);
    void onSonarSensorData(SonarRecord::Ptr record_);
    void onSoundSpeedSensorData(SoundSpeedRecord::Ptr record_);

    DataCallback            _callback;
    GnssInsRecordPtrOpt     _lastGnssRecord;
    SoundSpeedRecordPtrOpt  _currSoundSpeedRecord;
    bool                    _isDataConsistent;
  };
}
