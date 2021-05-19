#pragma once

#include "../../sensors/IGnssInsSensor.h"
#include "../../tools/parsing/FileStreamLineParser.h"
#include "../../tools/parsing/ThreadedStreamLineParser.h"
#include "../../tools/scheduling/ITimedExecutable.h"

namespace sp::playback::devices
{
  using namespace types;
  using namespace std::chrono_literals;

  class GnssInsSensorPlaybackDevice : public scheduling::ITimedExecutable, public sensors::IGnssInsSensor
  {
  public:
    GnssInsSensorPlaybackDevice(std::filesystem::path sensorDataFile_);

    virtual void setDataCallback(DataCallback&& callback_) override;

  private:
    using Record = sensors::data::GnssInsRecord;
    using Parser = tools::parsing::ThreadedStreamLineParser<tools::parsing::FileStreamLineParser<Record>, 20>;

    virtual scheduling::NextTimepoint getStartTime() override;
    virtual scheduling::NextTimepoint onExecute(Timepoint currentTime_) override;

    static constexpr Duration FRENQUENCY = 20ms;

    DataCallback  _callback;
    Parser        _parser;
  };
}