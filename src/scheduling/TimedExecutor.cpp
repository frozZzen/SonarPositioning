#include "TimedExecutor.h"

#include <algorithm>

using namespace sp::types;

namespace sp::scheduling
{
  TimedExecutor::TimedExecutor()
    : _running(false)
  {
  }

  TimedExecutor::~TimedExecutor()
  {
    stop();
  }

  TimedExecutor::TimedRunnable& TimedExecutor::addRunnable(TimedRunnable&& runnable_, std::optional<Timepoint> startTime_)
  {
    _runnables.emplace_back(std::move(runnable_), startTime_);
    return _runnables.back().first;
  }

  void TimedExecutor::start()
  {
    std::scoped_lock<std::mutex> guard{_mutex};
    if (!_running)
    {
      _running = true;
      const auto currentTime = Clock::now();
      for (auto& runnable : _runnables)
      {
        _runnableSchedule.emplace(runnable.second ? runnable.second.value() : currentTime, _runnables.back().first);
      }
      _thread = std::make_unique<std::thread>([this] { run(); });
    }
  }

  void TimedExecutor::wait()
  {
    std::scoped_lock<std::mutex> guard{_mutex};
    if (_running)
    {
      if (_thread && _thread->joinable()) { _thread->join(); }
      _thread.reset();
    }
  }
  
  void TimedExecutor::stop()
  {
    std::scoped_lock<std::mutex> guard{_mutex};
    if (_running)
    {
      _running = false;
      if (_thread && _thread->joinable()) { _thread->join(); }
      _thread.reset();
    }
  }

  void TimedExecutor::run()
  {
    while (_running && !_runnableSchedule.empty())
    {
      auto nextIt = _runnableSchedule.begin();
      const auto currentTime = Clock::now();
      const auto nextTime = nextIt->first;
      if (currentTime >= nextTime)
      {
        auto& func = nextIt->second.get();
        _runnableSchedule.erase(nextIt);
        auto nextTime = func(currentTime);
        if (nextTime)
        {
          _runnableSchedule.emplace(nextTime.value(), func);
        }
        std::this_thread::yield();
      }
      else
      {
        std::this_thread::sleep_for(nextTime - currentTime);
      }
      nextIt = _runnableSchedule.begin();
    }
  }
}