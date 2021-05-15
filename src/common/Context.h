#pragma once

#include "../sensors/IGnssInsSensor.h"
#include "../sensors/ISonarSensor.h"
#include "../sensors/ISoundSpeedSensor.h"
#include "../tools/ObjectRepo.h"

#include <functional>

namespace sp
{
  using ObjectRepo = tools::repo::ObjectRepo<
    tools::repo::TaggedObject<sensors::IGnssInsSensor, std::function<std::reference_wrapper<sensors::IGnssInsSensor> ()>>,
    tools::repo::TaggedObject<sensors::ISonarSensor, std::function<std::reference_wrapper<sensors::ISonarSensor> ()>>,
    tools::repo::TaggedObject<sensors::ISoundSpeedSensor, std::function<std::reference_wrapper<sensors::ISoundSpeedSensor> ()>>
  >;

  struct Context
  {
    ObjectRepo _objectRepo;
  };
}