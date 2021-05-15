#pragma once

#include "ITimedExecutable.h"

#include <atomic>
#include <functional>
#include <queue>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

namespace sp::scheduling
{
  using namespace types;

  class TimedExecutor
  {
  public:
    TimedExecutor(double executionTimeMultiplier_ = 1.0);
    TimedExecutor(TimedExecutor&&) = default;
    TimedExecutor& operator=(TimedExecutor&&) = default;
    TimedExecutor(const TimedExecutor&) = delete;
    TimedExecutor& operator=(const TimedExecutor&) = delete;
    ~TimedExecutor();

    void registerExecutable(ITimedExecutable& executable_);

    void start();
    void stop();
    void waitRunnablesAndStop();

  private:
    void stop(bool waitRunnableFinish_);
    void run();
    Duration getMultipliedDuration(Timepoint from_, Timepoint to_) const;

    using ExecutableRef = std::reference_wrapper<ITimedExecutable>;

    class ScheduleEntry
    {
    public:
      ScheduleEntry(Timepoint timepoint_, ExecutableRef executable_)
        : _timepoint(timepoint_), _executable(executable_) {}

      bool operator<(const ScheduleEntry& o_) const { return _timepoint > o_._timepoint; }

      Timepoint _timepoint;
      ExecutableRef _executable;
    };

    using Schedule = std::priority_queue<ScheduleEntry>;

    std::vector<ExecutableRef>    _executables;
    double                        _executionTimeMultiplier;
    Schedule                      _schedule;
    std::atomic_bool              _running;
    std::mutex                    _mutex;
    std::unique_ptr<std::thread>  _thread;
    std::optional<Timepoint>      _lastExecutionTime;
  };
}
