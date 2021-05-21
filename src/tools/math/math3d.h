#pragma once

#include "../../common/types.h"

#include <ostream>

namespace sp::tools::math
{
  using namespace types;

  struct Angle3d
  {
    Angle3d(Radian roll_, Radian pitch_, Radian yaw_);

    Radian _x, _y, _z;
  };

  template <typename T>
  struct Vector3d
  {
    Vector3d();
    Vector3d(T x_, T y_, T z_);

    T _x, _y, _z;
  };

  template <typename T>
  struct Matrix3d
  {
    Vector3d<T> _c1, _c2, _c3;
  };

  template <typename T> Vector3d<T> operator+(const Vector3d<T>& v1_, const Vector3d<T>& v2_);
  template <typename T> Vector3d<T> operator-(const Vector3d<T>& v1_, const Vector3d<T>& v2_);

  template <typename T> Vector3d<T> transform(const Matrix3d<T>& m_, const Vector3d<T>& v_);
  template <typename T> Vector3d<T> operator|(const Matrix3d<T>& m_, const Vector3d<T>& v_);

  template <typename T> Matrix3d<T> compose(const Matrix3d<T>& m2_, const Matrix3d<T>& m1_);
  template <typename T> Matrix3d<T> operator*(const Matrix3d<T>& m2_, const Matrix3d<T>& m1_);

  template <typename T> Matrix3d<T> getRotationMatrix(Angle3d angle_);

  template <typename T> Vector3d<T> rotate(const Vector3d<T>& v_, Angle3d angle_);
  template <typename T> Vector3d<T> operator|(const Vector3d<T>& v_, Angle3d angle_);
}

namespace std
{
  ostream& operator<<(ostream& stream_, const ::sp::tools::math::Angle3d& a_);

  template <typename T>
  ostream& operator<<(ostream& stream_, const ::sp::tools::math::Vector3d<T>& v_);

  template <typename T>
  ostream& operator<<(ostream& stream_, const ::sp::tools::math::Matrix3d<T>& m_);
}

#include "math3d.hpp"
