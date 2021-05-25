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
      calculateTimestamp(*_prevRecord.value());
      return _prevRecord.value()->_timestamp;
    }
    return std::nullopt;
  }

  scheduling::NextTimepoint SonarSensorPlaybackDevice::onExecute(Timepoint currentTime_)
  {
    if (_parser.hasMoreData() && _callback && _prevRecord)
    {
      const auto prevTimestamp = _prevRecord.value()->_timestamp;
      try
      {
        _callback(std::move(_prevRecord.value()));
      }
      catch (std::exception& e_)
      {
        logError("Exception: ", e_.what());
      }
      auto record = _parser.getNextData();
      if (record)
      {
        calculateTimestamp(*record.value());
        Duration diff = record.value()->_timestamp - prevTimestamp;
        _prevRecord = std::move(record.value());
        return currentTime_ + diff;
      }
    }
    return std::nullopt;
  }

  void SonarSensorPlaybackDevice::calculateTimestamp(Record& record_)
  {
    const auto maxSample = std::max_element(record_._samples.begin(), record_._samples.end(),
      [](const auto& s1_, const auto& s2_) { return s1_._sampleIndex < s2_._sampleIndex; });
    const auto maxSampleDelay = 1.0 * maxSample->_sampleIndex / record_._samplingRate;

    record_._timestamp = record_._samplingStartTime + tools::time_conversion::toDuration(maxSampleDelay);
  }
}
