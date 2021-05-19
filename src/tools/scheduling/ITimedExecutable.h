#pragma once

#include "../../common/types.h"

#include <optional>
#include <variant>

namespace sp::scheduling
{
  using namespace types;

  using NextTimepoint = std::optional<Timepoint>;

  class ITimedExecutable
  {
  public:
    using NextTimepoint = sp::scheduling::NextTimepoint;

    virtual NextTimepoint getStartTime() = 0;
    virtual NextTimepoint onExecute(Timepoint currentTime_) = 0;

    virtual ~ITimedExecutable() = default;
  };
}
