#include "SonarSensorPlaybackDevice.h"

#include "../../common/log.h"
#include "../../sensors/data/parsing/SonarRecordParser.h"

namespace sp::playback::devices
{
  SonarSensorPlaybackDevice::SonarSensorPlaybackDevice(std::filesystem::path sensorDataFile_)
    : _parser(std::move(sensorDataFile_), sensors::data::parsing::parseSonarRecord)
  {
  }

  void SonarSensorPlaybackDevice::setDataCallback(DataCallback&& callback_)
  {
    _callback = std::move(callback_);
  }

  scheduling::NextTimepoint SonarSensorPlaybackDevice::getStartTime()
  {
    _prevRecord = _parser.getNextData();
    if (_prevRecord)
    {
      return _prevRecord.value()._timestamp;
    }
    return std::nullopt;
  }

  scheduling::NextTimepoint SonarSensorPlaybackDevice::onExecute(Timepoint currentTime_)
  {
    if (_parser.hasMoreData() && _callback && _prevRecord)
    {
      try
      {
        _callback(_prevRecord.value());
      }
      catch (std::exception& e_)
      {
        logError("Exception: ", e_.what());
      }
      auto record = _parser.getNextData();
      if (record)
      {
        Duration diff = record.value()._timestamp - _prevRecord.value()._timestamp;
        _prevRecord = std::move(record);
        return currentTime_ + diff;
      }
    }
    return std::nullopt;
  }
}
