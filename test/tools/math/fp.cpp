#include "../../pch.h"

#include <tools/math/fp.h>

using namespace sp::tools::math;

TEST(Math, FloatingPointCompare)
{
  double left1 = 0.000'000'1;
  double right1 = 0.000'000'2;
  EXPECT_TRUE(Compare{left1} == right1);
  EXPECT_FALSE(Compare{left1} < right1);
  EXPECT_FALSE(Compare{left1} > right1);
  EXPECT_TRUE(Compare{left1} <= right1);
  EXPECT_TRUE(Compare{left1} >= right1);

  EXPECT_FALSE(Compare(left1, 0.5e-7) == right1);
  EXPECT_TRUE(Compare(left1, 0.5e-7) < right1);
  EXPECT_FALSE(Compare(left1, 0.5e-7) > right1);
  EXPECT_TRUE(Compare(left1, 0.5e-7) <= right1);
  EXPECT_FALSE(Compare(left1, 0.5e-7) >= right1);

  double left, right;
  left = 0.000'001;
  right = 0.000'002;
  EXPECT_FALSE(Compare{left} == right);
  EXPECT_TRUE(Compare{left} < right);
  EXPECT_FALSE(Compare{left} > right);
  EXPECT_TRUE(Compare{left} <= right);
  EXPECT_FALSE(Compare{left} >= right);

  left = 1'000'000.1;
  right = 1'000'000.2;
  EXPECT_TRUE(Compare{left} == right);
  EXPECT_FALSE(Compare{left} < right);
  EXPECT_FALSE(Compare{left} > right);
  EXPECT_TRUE(Compare{left} <= right);
  EXPECT_TRUE(Compare{left} >= right);

  left = 1'000'001.0;
  right = 1'000'002.0;
  EXPECT_FALSE(Compare{left} == right);
  EXPECT_TRUE(Compare{left} < right);
  EXPECT_FALSE(Compare{left} > right);
  EXPECT_TRUE(Compare{left} <= right);
  EXPECT_FALSE(Compare{left} >= right);
}
