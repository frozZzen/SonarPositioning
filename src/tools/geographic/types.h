#pragma once

#include "../../common/types.h"
#include "../math/math3d.h"

#include <ostream>
#include <string>

namespace sp::tools::geographic
{
  using namespace types;
  using namespace tools::math;

  struct CartFlatPos
  {
    CartFlatPos() : _x(0), _y(0) {}
    CartFlatPos(Meter x_, Meter y_)
      : _x(x_), _y(y_) {}

    Meter _x, _y;
  };

  struct CartPos : CartFlatPos
  {
    CartPos() : CartFlatPos(), _z(0) {}
    CartPos(Vector3d<Meter> v_) : CartFlatPos(v_._x, v_._y), _z(v_._z) {}
    CartPos(Meter x_, Meter y_, Meter z_)
      : CartFlatPos(x_, y_), _z(z_) {}

    operator Vector3d<Meter>() const { return {_x, _y, _z}; }

    Meter _z;
  };

  struct GeoFlatPos
  {
    GeoFlatPos() : _lat(0), _lon(0) {}
    GeoFlatPos(double lat_, double lon_)
      : _lat(lat_), _lon(lon_) {}

    double _lat, _lon;
  };

  struct GeoPos : GeoFlatPos
  {
    GeoPos() : GeoFlatPos(), _alt(0) {}
    GeoPos(double lat_, double lon_, Meter alt_)
      : GeoFlatPos(lat_, lon_), _alt(alt_) {}

    Meter _alt;
  };

  struct UtmUps : CartFlatPos
  {
    UtmUps(const GeoFlatPos& pos_);
    UtmUps(Radian lat_, Radian lon_);
    UtmUps(int zone_, bool northp_, Radian x_, Radian y_)
      : CartFlatPos(x_, y_), _zone(zone_), _northp(northp_) {}

    std::string zone() const;

    int   _zone;
    bool  _northp;
  };
}

namespace std
{
  inline ostream& operator<<(ostream& stream_, const ::sp::tools::geographic::CartFlatPos& pos_)
  {
    return stream_ << "CartFlatPos{x: " << pos_._x << ", y: " << pos_._y << '}';
  }

  inline ostream& operator<<(ostream& stream_, const ::sp::tools::geographic::CartPos& pos_)
  {
    return stream_ << "CartPos{x: " << pos_._x << ", y: " << pos_._y << ", z: " << pos_._z << '}';
  }

  inline ostream& operator<<(ostream& stream_, const ::sp::tools::geographic::GeoFlatPos& pos_)
  {
    return stream_ << "GeoFlatPos{Lat: " << pos_._lat << ", Lon: " << pos_._lon << '}';
  }

  inline ostream& operator<<(ostream& stream_, const ::sp::tools::geographic::GeoPos& pos_)
  {
    return stream_ << "GeoPos{Lat: " << pos_._lat << ", Lon: " << pos_._lon << ", Alt: " << pos_._alt << '}';
  }

  inline ostream& operator<<(ostream& stream_, const ::sp::tools::geographic::UtmUps& pos_)
  {
    return stream_ << "UtmUps{" << pos_.zone() << ' ' << pos_._x << ' ' << pos_._y << '}';
  }
}
