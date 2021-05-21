#include "../pch.h"

#include <common/log.h>
#include <common/Context.h>
#include <positioning/SonarPositioner.h>
#include <tools/geographic/types.h>

#include <fstream>

using namespace sp;
using namespace sp::playback;
using namespace sp::playback::devices;
using namespace sp::positioning;
using namespace sp::sensors;
using namespace sp::tools::geographic;

TEST(SonarPositionerDemo, DemoOnly)
{
  constexpr double EXECUTION_TIME_MULTIPLIER = 0.0;

  constexpr const char* GNSS_DATA_PATH    = "test_data/gnss.txt";
  constexpr const char* SONAR_DATA_PATH   = "test_data/sonar.txt";
  constexpr const char* SOUND_DATA_PATH   = "test_data/speed_of_sound.txt";

  constexpr const char* OUTPUT_DATA_PATH  = "output_data/out.txt";

  Context context;
  SonarPositionerPlaybackController::Config controllerConfig{
    context, GNSS_DATA_PATH, SONAR_DATA_PATH, SOUND_DATA_PATH, EXECUTION_TIME_MULTIPLIER
  };

  SonarPositionerPlaybackController controller{controllerConfig};
  SonarPositioner positioner{context};

  logInfo("Writing data to output file: ", OUTPUT_DATA_PATH);
  std::fstream outputFile;
  outputFile.open(OUTPUT_DATA_PATH, std::fstream::out);

  auto dataCallback = [&](const GeoPos sonarPos_, const std::vector<GeoPos>& positions_) {
    logDebug("Sonar Geodetic: ", sonarPos_);
    outputFile << "Sonar Geodetic: " << sonarPos_ << '\n';
    for (const auto& pos : positions_)
    {
      const UtmUps utmUps{pos};
      logDebug("  ", pos, ", ", utmUps);
      outputFile << "  " << pos << ", " << utmUps << '\n';
    }
  };
  positioner.setDataCallback(dataCallback);

  controller.start();
  controller.waitPlaybackAndStop();

  outputFile.flush();
  outputFile.close();
}
