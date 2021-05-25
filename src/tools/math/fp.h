#pragma once

#include <limits>
#include <type_traits>

namespace sp::tools::math
{
  template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
  struct Compare
  {
    constexpr explicit Compare(T value_, T epsilon_ = 0.5e-6) : _value{value_}, _epsilon(epsilon_) {}

    constexpr bool operator<(T o_) const  { return (_value < o_) && (*this != o_); }
    constexpr bool operator>(T o_) const  { return _value > o_ && *this != o_; }
    constexpr bool operator<=(T o_) const { return _value < o_ || *this == o_; }
    constexpr bool operator>=(T o_) const { return _value > o_ || *this == o_; }
    constexpr bool operator!=(T o_) const { return !(*this == o_); }
    constexpr bool operator==(T o_) const
    {
      T delta = std::abs(_value - o_);
      return delta <= _epsilon || delta <= _epsilon * std::max(std::abs(_value), std::abs(o_));
    }

    T _value;
    T _epsilon;
  };
}
