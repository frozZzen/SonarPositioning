#pragma once

#include "../../sensors/ISoundSpeedSensor.h"
#include "../../tools/parsing/FileStreamLineParser.h"
#include "../../tools/parsing/ThreadedStreamLineParser.h"
#include "../../tools/scheduling/ITimedExecutable.h"

namespace sp::playback::devices
{
  using namespace types;
  using namespace std::chrono_literals;

  class SoundSpeedSensorPlaybackDevice : public scheduling::ITimedExecutable, public sensors::ISoundSpeedSensor
  {
  public:
    SoundSpeedSensorPlaybackDevice(std::filesystem::path sensorDataFile_);

    virtual void setDataCallback(DataCallback&& callback_) override;

  private:
    using Record = sensors::data::SoundSpeedRecord;
    using Parser = tools::parsing::ThreadedStreamLineParser<tools::parsing::FileStreamLineParser<Record::Ptr>, 100>;

    virtual scheduling::NextTimepoint getStartTime() override;
    virtual scheduling::NextTimepoint onExecute(Timepoint currentTime_) override;

    static constexpr Duration FRENQUENCY = 1s;

    DataCallback  _callback;
    Parser        _parser;
  };
}