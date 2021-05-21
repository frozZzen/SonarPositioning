#include "types.h"

#include <GeographicLib/UTMUPS.hpp>

namespace sp::tools::geographic
{
  UtmUps::UtmUps(const GeoFlatPos& pos_)
    : UtmUps(pos_._lat, pos_._lon)
  {
  }

  UtmUps::UtmUps(Radian lat_, Radian lon_)
    : _zone(), _northp()
  {
    GeographicLib::UTMUPS::Forward(lat_, lon_, _zone, _northp, _x, _y);
  }

  std::string UtmUps::zone() const
  {
    return GeographicLib::UTMUPS::EncodeZone(_zone, _northp);
  }
}
