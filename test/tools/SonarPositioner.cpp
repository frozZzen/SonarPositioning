#include "../pch.h"

#include <common/log.h>
#include <common/Context.h>
#include <positioning/SonarPositioner.h>

#include <GeographicLib/GeoCoords.hpp>

using namespace sp;
using namespace sp::playback;
using namespace sp::playback::devices;
using namespace sp::positioning;
using namespace sp::sensors;

TEST(SonarPositionerDemo, DemoOnly)
{
  constexpr double EXECUTION_TIME_MULTIPLIER = 0.2;

  constexpr char* GNSS_DATA_PATH    = "test_data/gnss.txt";
  constexpr char* SONAR_DATA_PATH   = "test_data/sonar.txt";
  constexpr char* SOUND_DATA_PATH   = "test_data/speed_of_sound.txt";

  Context context;
  SonarPositionerPlaybackController::Config controllerConfig{
    context, GNSS_DATA_PATH, SONAR_DATA_PATH, SOUND_DATA_PATH, EXECUTION_TIME_MULTIPLIER
  };

  SonarPositionerPlaybackController controller{controllerConfig};
  SonarPositioner positioner{context};

  auto dataCallback = [](const GeoPos sonarPos_, const std::vector<GeoPos>& positions_) {
    logInfo("Sonar Geodetic: ", sonarPos_);
    for (const auto& pos : positions_)
    {
      GeographicLib::GeoCoords geoCoords{pos._lat, pos._lon};
      logInfo("Geodetic: ", pos);
      logInfo("UTMUPS: ", geoCoords.UTMUPSRepresentation(false));
    }
  };
  positioner.setDataCallback(dataCallback);

  controller.start();
  controller.waitPlaybackAndStop();
}
