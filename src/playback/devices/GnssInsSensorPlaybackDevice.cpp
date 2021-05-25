#include "GnssInsSensorPlaybackDevice.h"

#include "../../common/log.h"
#include "../../sensors/data/parsing/GnssInsRecordParser.h"

namespace sp::playback::devices
{
  GnssInsSensorPlaybackDevice::GnssInsSensorPlaybackDevice(std::filesystem::path sensorDataFile_)
    : _parser(std::move(sensorDataFile_), sensors::data::parsing::parseGnssInsRecord)
  {
  }

  void GnssInsSensorPlaybackDevice::setDataCallback(DataCallback&& callback_)
  {
    _callback = std::move(callback_);
  }

  scheduling::NextTimepoint GnssInsSensorPlaybackDevice::getStartTime()
  {
    return std::nullopt;
  }

  scheduling::NextTimepoint GnssInsSensorPlaybackDevice::onExecute(Timepoint currentTime_)
  {
    if (_parser.hasMoreData() && _callback)
    {
      auto record = _parser.getNextData();
      if (record)
      {
        record.value()->_timestamp = currentTime_;
        try
        {
          _callback(std::move(record.value()));
        }
        catch (std::exception& e_)
        {
          logError("Exception received from data callback: ", e_.what());
        }
        return currentTime_ + FRENQUENCY;
      }
    }
    return std::nullopt;
  }
}
