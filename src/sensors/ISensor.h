#pragma once

#include <functional>
#include <memory>

namespace sp::sensors
{
  template <typename RecordT>
  class ISensor
  {
  public:
    using Record = RecordT;
    using DataCallback = std::function<void (typename Record::Ptr)>;

    virtual void setDataCallback(DataCallback&& callback_) = 0;

    virtual ~ISensor() = default;
  };
}
