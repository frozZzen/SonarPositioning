#include "SoundSpeedSensorPlaybackDevice.h"

#include "../../common/log.h"
#include "../../sensors/data/parsing/SoundSpeedRecord.h"

namespace sp::playback::devices
{
  SoundSpeedSensorPlaybackDevice::SoundSpeedSensorPlaybackDevice(std::filesystem::path sensorDataFile_)
    : _parser(std::move(sensorDataFile_), sensors::data::parsing::parseSoundSpeedRecord)
  {
  }

  void SoundSpeedSensorPlaybackDevice::setDataCallback(DataCallback&& callback_)
  {
    _callback = std::move(callback_);
  }

  scheduling::NextTimepoint SoundSpeedSensorPlaybackDevice::getStartTime()
  {
    return std::nullopt;
  }

  scheduling::NextTimepoint SoundSpeedSensorPlaybackDevice::onExecute(Timepoint currentTime_)
  {
    if (_parser.hasMoreData() && _callback)
    {
      auto record = _parser.getNextData();
      if (record)
      {
        record.value()._timestamp = currentTime_;
        try
        {
          _callback(record.value());
        }
        catch (std::exception& e_)
        {
          logError("Exception: ", e_.what());
        }
        return currentTime_ + FRENQUENCY;
      }
    }
    return std::nullopt;
  }
}
