#pragma once

#include <tools/scheduling/ITimedExecutable.h>

#include <functional>

namespace sp::helpers
{
  using namespace types;

  class ExecutableMock : public scheduling::ITimedExecutable
  {
  public:
    using Callback = std::function<NextTimepoint (Timepoint)>;

    ExecutableMock(Callback executeCallback_, NextTimepoint startTime_)
      : _callback(std::move(executeCallback_)), _startTime(startTime_) {}

    NextTimepoint getStartTime() override { return _startTime; }
    NextTimepoint onExecute(Timepoint currentTime_) override { return _callback(currentTime_); }

  private:
    Callback          _callback;
    NextTimepoint     _startTime;
  };
}
