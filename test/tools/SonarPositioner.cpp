#include "../pch.h"

#include <common/log.h>
#include <common/Context.h>
#include <positioning/SonarPositioner.h>

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

  controller.start();
  controller.waitPlaybackAndStop();
}
