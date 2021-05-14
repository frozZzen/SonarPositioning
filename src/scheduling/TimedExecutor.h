#pragma once

#include "../common/types.h"

#include <atomic>
#include <functional>
#include <map>
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
    using TimedRunnable = std::function<std::optional<Timepoint> (Timepoint)>;

    TimedExecutor();
    TimedExecutor(TimedExecutor&&) = default;
    TimedExecutor& operator=(TimedExecutor&&) = default;
    TimedExecutor(const TimedExecutor&) = delete;
    TimedExecutor& operator=(const TimedExecutor&) = delete;
    ~TimedExecutor();

    TimedRunnable& addRunnable(TimedRunnable&& runnable_, std::optional<Timepoint> startTime_);

    void start();
    void wait();
    void stop();

  private:
    void run();

    using RunnableStartTimePair = std::pair<TimedRunnable, std::optional<Timepoint>>;
    using TimeToRunnableMap = std::map<Timepoint, std::reference_wrapper<TimedRunnable>>;

    std::vector<RunnableStartTimePair> _runnables;
    TimeToRunnableMap             _runnableSchedule;
    std::atomic_bool              _running;
    std::mutex                    _mutex;
    std::unique_ptr<std::thread>  _thread;
  };
}
