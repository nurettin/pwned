#include <gtest/gtest.h>
#include "progress.hpp"

TEST(PwnedProgress, DurationParts)
{
  pwned::progress::DurationParts dp(boost::chrono::milliseconds(12312312));
  EXPECT_EQ(3, dp.h.count());
  EXPECT_EQ(25, dp.m.count());
  EXPECT_EQ(12, dp.s.count());
  EXPECT_EQ(312, dp.ms.count());
}

TEST(PwnedProgress, Estimate)
{
  pwned::progress::Progress p(10);
  p.elapsed_milliseconds= 10000;
  p.ticks= 5;
  EXPECT_EQ(10000, p.estimate().count());
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

