#include "../../pch.h"

#include <common/log.h>
#include <tools/math/math3d.h>

using namespace sp::tools::math;

TEST(Math, Math3dRotation)
{
  constexpr auto deg2Rad = [](double deg_) { return deg_ * 3.14159265359 / 180; };

  Vector3d<double> xUnit{1, 0, 0};
  Vector3d<double> yUnit{0, 1, 0};
  Vector3d<double> zUnit{0, 0, 1};

  Angle3d xAngle{deg2Rad(90), 0, 0};
  Angle3d yAngle{0, deg2Rad(90), 0};
  Angle3d zAngle{0, 0, deg2Rad(90)};

  EXPECT_EQ(xUnit | xAngle, xUnit);
  EXPECT_EQ(xUnit | yAngle, -1.0 * zUnit);
  EXPECT_EQ(xUnit | zAngle, yUnit);

  EXPECT_EQ(yUnit | xAngle, zUnit);
  EXPECT_EQ(yUnit | yAngle, yUnit);
  EXPECT_EQ(yUnit | zAngle, -1.0 * xUnit);

  EXPECT_EQ(zUnit | xAngle, -1.0 * yUnit);
  EXPECT_EQ(zUnit | yAngle, xUnit);
  EXPECT_EQ(zUnit | zAngle, zUnit);
}

TEST(Math, Math3dTranslation)
{
  Vector3d<double> pos1{-1, 2, 3};
  Vector3d<double> pos2{4, -5, 6};
  Vector3d<double> exp1{3, -3, 9};

  EXPECT_EQ(pos1 + pos2, exp1);

  Vector3d<double> pos3{-1, -3, 6};
  Vector3d<double> pos4{1, 6, 3};
  Vector3d<double> exp2{-2, -9, 3};

  EXPECT_EQ(pos3 - pos4, exp2);
}
