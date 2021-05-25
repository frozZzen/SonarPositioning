#include "TimedExecutor.h"

#include "../../common/log.h"
#include "../time_conversion.h"

using namespace sp::types;
using namespace std::chrono_literals;

namespace sp::scheduling
{
  TimedExecutor::TimedExecutor(double executionTimeMultiplier_)
    : _executionTimeMultiplier(executionTimeMultiplier_)
    , _running(false)
  {
  }

  TimedExecutor::~TimedExecutor()
  {
    stop();
  }

  void TimedExecutor::registerExecutable(ITimedExecutable& executable_)
  {
    _executables.emplace_back(executable_);
  }

  void TimedExecutor::start()
  {
    std::scoped_lock<std::mutex> guard{_mutex};
    if (!_running.load() && !_executables.empty())
    {
      _running.store(true);
      std::vector<ExecutableRef> _executablesWithNoStartTime;
      for (auto& executable : _executables)
      {
        auto startTime = executable.get().getStartTime();
        if (startTime)
        {
          _schedule.emplace(startTime.value(), executable);
        }
        else
        {
          _executablesWithNoStartTime.push_back(executable);
        }
      }

      Timepoint minStartTime = !_schedule.empty() ? _schedule.top()._timepoint : Clock::now();
      for (auto& executable : _executablesWithNoStartTime)
      {
        _schedule.emplace(minStartTime, executable);
      }
      _thread = std::make_unique<std::thread>([this] { run(); });
    }
  }

  void TimedExecutor::stop()
  {
    stop(false);
  }

  void TimedExecutor::waitExecutablesAndStop()
  {
    stop(true);
  }
  
  void TimedExecutor::stop(bool waitRunnableFinish_)
  {
    std::scoped_lock<std::mutex> guard{_mutex};
    if (_running.load())
    {
      _running.store(waitRunnableFinish_);
      if (_thread && _thread->joinable()) { _thread->join(); }
      _thread.reset();
      _running.store(false);
    }
  }

  void TimedExecutor::run()
  {
    while (_running)
    {
      auto scheduleEntry = _schedule.top();
      _schedule.pop();
      try
      {
        auto nextTime = scheduleEntry._executable.get().onExecute(scheduleEntry._timepoint);
        if (nextTime)
        {
          _schedule.emplace(nextTime.value(), scheduleEntry._executable);
        }
      }
      catch (std::exception& e_)
      {
        logError("Exception received from onExecute: ", e_.what());
      }
      if (_schedule.empty()) [[unlikely]]
      {
        return;
      }
      auto nextDuration = getMultipliedDuration(scheduleEntry._timepoint, _schedule.top()._timepoint);
      std::this_thread::sleep_for(nextDuration);
    }
  }

  Duration TimedExecutor::getMultipliedDuration(Timepoint from_, Timepoint to_) const
  {
    if (_executionTimeMultiplier < 0.001 || to_ <= from_)
    {
      return Duration{0};
    }
    return Duration{static_cast<int64_t>((to_ - from_).count() * _executionTimeMultiplier)};
  }
}