#pragma once

#include "../../scheduling/ITimedExecutable.h"
#include "../../sensors/ISonarSensor.h"
#include "../../tools/parsing/FileStreamLineParser.h"
#include "../../tools/parsing/ThreadedStreamLineParser.h"

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
    using Record = sensors::data::SonarRecord;
    using RecordOpt = std::optional<Record>;
    using Parser = tools::parsing::ThreadedStreamLineParser<tools::parsing::FileStreamLineParser<Record>, 100>;

    virtual scheduling::NextTimepoint getStartTime() override;
    virtual scheduling::NextTimepoint onExecute(Timepoint currentTime_) override;

    DataCallback  _callback;
    Parser        _parser;
    RecordOpt     _prevRecord;
    NextTimepoint _initTime;
  };
}