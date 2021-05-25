#pragma once

#include "../../sensors/ISonarSensor.h"
#include "../../tools/parsing/FileStreamLineParser.h"
#include "../../tools/parsing/ThreadedStreamLineParser.h"
#include "../../tools/scheduling/ITimedExecutable.h"

namespace sp::playback::devices
{
  using namespace types;
  using namespace std::chrono_literals;

  class SonarSensorPlaybackDevice : public scheduling::ITimedExecutable, public sensors::ISonarSensor
  {
  public:
    SonarSensorPlaybackDevice(std::filesystem::path sensorDataFile_);

    virtual void setDataCallback(DataCallback&& callback_) override;

  private:
    using Record        = sensors::data::SonarRecord;
    using RecordPtrOpt  = std::optional<Record::Ptr>;
    using Parser        = tools::parsing::ThreadedStreamLineParser<tools::parsing::FileStreamLineParser<Record::Ptr>, 100>;

    virtual scheduling::NextTimepoint getStartTime() override;
    virtual scheduling::NextTimepoint onExecute(Timepoint currentTime_) override;

    void calculateTimestamp(Record& record_);

    DataCallback  _callback;
    Parser        _parser;
    RecordPtrOpt  _prevRecord;
    NextTimepoint _initTime;
  };
}