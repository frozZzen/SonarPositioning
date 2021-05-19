#include "../../pch.h"

#include "TimedExecutorHelpers.h"

#include <tools/scheduling/TimedExecutor.h>

#include <chrono>

using namespace sp::helpers;
using namespace sp::scheduling;
using namespace sp::types;

using namespace std::chrono_literals;

TEST(TimedExecutor, TestTwoRunnables)
{
  constexpr size_t periodCount1 = 4;
  constexpr size_t periodCount2 = 2;

  constexpr Duration period1 = 200ms;
  constexpr Duration period2 = 350ms;

  constexpr std::optional<Duration> startTimeOffset1 = std::nullopt;
  constexpr std::optional<Duration> startTimeOffset2 = 100ms;

  constexpr Duration periodTolerance = 50ms;

  
  std::vector<Duration> ExecutablePeriods1{periodCount1, period1};
  std::vector<Duration> ExecutablePeriods2{periodCount2, period2};

  std::vector<Timepoint> ReceivedCurrTimes1;
  std::vector<Timepoint> ReceivedCurrTimes2;

  auto executableBody = [&](Timepoint curr_, auto& periods_, auto& receivedTimes_) {
    if (periods_.empty()) { return std::optional<Timepoint>{}; }

    receivedTimes_.push_back(curr_);
    auto nextTime = curr_ + periods_.back();
    periods_.pop_back();

    return std::make_optional(nextTime);
  };

  const auto startTime = Clock::now();
  NextTimepoint startTime1 = startTimeOffset1 ? startTime + startTimeOffset1.value() : NextTimepoint{};
  NextTimepoint startTime2 = startTimeOffset2 ? startTime + startTimeOffset2.value() : NextTimepoint{};

  ExecutableMock executable1{[&](auto time_) { return executableBody(time_, ExecutablePeriods1, ReceivedCurrTimes1); }, startTime1};
  ExecutableMock executable2{[&](auto time_) { return executableBody(time_, ExecutablePeriods2, ReceivedCurrTimes2); }, startTime2};

  TimedExecutor executor;
  executor.registerExecutable(executable1);
  executor.registerExecutable(executable2);

  executor.start();
  executor.waitExecutablesAndStop();

  EXPECT_EQ(ReceivedCurrTimes1.size(), periodCount1);
  EXPECT_EQ(ReceivedCurrTimes2.size(), periodCount2);
  
  auto verifyTimePeriods = [&](const auto& receivedTimes_, Duration expectedPeriod_, NextTimepoint expectedStartTime_) {
    std::optional<Timepoint> prevTime;
    for (const auto& time : receivedTimes_)
    {
      if (prevTime)
      {
        const auto period = time - prevTime.value();
        EXPECT_LE(expectedPeriod_ - periodTolerance, period);
        EXPECT_GE(expectedPeriod_ + periodTolerance, period);
      }
      else
      {
        auto start = expectedStartTime_ ? expectedStartTime_.value() : startTime;
        EXPECT_LE(start - periodTolerance, time);
        EXPECT_GE(start + periodTolerance, time);
      }
      prevTime = time;
    }
  };

  verifyTimePeriods(ReceivedCurrTimes1, period1, startTime2);
  verifyTimePeriods(ReceivedCurrTimes2, period2, startTime2);


}
