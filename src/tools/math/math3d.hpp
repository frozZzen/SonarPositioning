#include <cmath>

namespace sp::tools::math
{
  inline Angle3d::Angle3d(Radian roll_, Radian pitch_, Radian yaw_)
    : _x(roll_), _y(pitch_), _z(yaw_)
  {
  }

  template <typename T>
  Vector3d<T>::Vector3d()
    : _x(0), _y(0), _z(0)
  {
  }

  template <typename T>
  Vector3d<T>::Vector3d(T x_, T y_, T z_)
    : _x(x_), _y(y_), _z(z_)
  {
  }

  template <typename T>
  Vector3d<T> operator+<T>(const Vector3d<T>& v1_, const Vector3d<T>& v2_)
  {
    return {v1_._x + v2_._x, v1_._y + v2_._y, v1_._z + v2_._z};
  }

  template <typename T>
  Vector3d<T> operator-<T>(const Vector3d<T>& v1_, const Vector3d<T>& v2_)
  {
    return {v1_._x - v2_._x, v1_._y - v2_._y, v1_._z - v2_._z};
  }

  template <typename T>
  Vector3d<T> transform<T>(const Matrix3d<T>& m_, const Vector3d<T>& v_)
  {
    return {
      v_._x * m_._c1._x + v_._y * m_._c2._x + v_._z * m_._c3._x,
      v_._x * m_._c1._y + v_._y * m_._c2._y + v_._z * m_._c3._y,
      v_._x * m_._c1._z + v_._y * m_._c2._z + v_._z * m_._c3._z,
    };
  }

  template <typename T>
  Vector3d<T> operator|<T>(const Matrix3d<T>& m_, const Vector3d<T>& v_)
  {
    return transform(m_, v_);
  }

  template <typename T>
  Matrix3d<T> compose<T>(const Matrix3d<T>& m2_, const Matrix3d<T>& m1_)
  {
    return {m2_ | m1_._c1, m2_ | m1_._c2, m2_ | m1_._c3};
  }

  template <typename T>
  Matrix3d<T> operator*<T>(const Matrix3d<T>& m2_, const Matrix3d<T>& m1_)
  {
    return compose(m2_, m1_);
  }

  template <typename T>
  Matrix3d<T> getRotationMatrix<T>(Angle3d angle_)
  {
    using namespace std;
    const double cosx = cos(angle_._x), sinx = sin(angle_._x);
    const double cosy = cos(angle_._y), siny = sin(angle_._y);
    const double cosz = cos(angle_._z), sinz = sin(angle_._z);
    return {
      {cosz * cosy,                      sinz * cosy,                      -siny       },
      {cosz * siny * sinx - sinz * cosx, sinz * siny * sinx + cosz * cosx,  cosy * sinx},
      {cosz * siny * cosx + sinz * sinx, sinz * siny * cosx - cosz * sinx,  cosy * cosx},
    };
  }

  template <typename T>
  Vector3d<T> rotate<T>(const Vector3d<T>& v_, Angle3d angle_)
  {
    return getRotationMatrix<T>(angle_) | v_;
  }

  template <typename T>
  Vector3d<T> operator|<T>(const Vector3d<T>& v_, Angle3d angle_)
  {
    return rotate(v_, angle_);
  }
}

namespace std
{
  inline ostream& operator<<(ostream& stream_, const ::sp::tools::math::Angle3d& a_)
  {
    return stream_ << "Angle3d{Roll(x): " << a_._x << ", Pitch(y): " << a_._y << ", Yaw(z): " << a_._z << '}';
  }

  template <typename T>
  ostream& operator<<<T>(ostream& stream_, const ::sp::tools::math::Vector3d<T>& v_)
  {
    return stream_ << "Vector3d{x: " << v_._x << ", y: " << v_._y << ", z: " << v_._z << '}';
  }

  template <typename T>
  ostream& operator<<<T>(ostream& stream_, const ::sp::tools::math::Matrix3d<T>& m_)
  {
    return stream_ << "Matrix3d{"
      "x0: " << m_._c1._x << ", x1: " << m_._c2._x << ", x2: " << m_._c3._x <<
      "y0: " << m_._c1._y << ", y1: " << m_._c2._y << ", y2: " << m_._c3._y <<
      "z0: " << m_._c1._z << ", z1: " << m_._c2._z << ", z2: " << m_._c3._z << '}';
  }
}
