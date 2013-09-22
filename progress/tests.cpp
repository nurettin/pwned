#include <sstream>
#include <algorithm>
#include <gtest/gtest.h>
#include <chrono>
#include "progress.hpp"

TEST(PwnedProgress, DurationParts)
{
  pwned::progress::DurationParts dp(std::chrono::milliseconds(12312312));
  EXPECT_EQ(3, dp.h.count());
  EXPECT_EQ(25, dp.m.count());
  EXPECT_EQ(12, dp.s.count());
  EXPECT_EQ(312, dp.ms.count());
  std::ostringstream os;
  os<< dp;
  EXPECT_EQ("03:25:12.312", os.str());
}

TEST(PwnedProgress, Estimate)
{
  pwned::progress::Progress p(10);
  p.elapsed_milliseconds= 10000;
  p.ticks= 5;
  EXPECT_EQ(10000, p.estimate().count());
}

TEST(PwnedProgress, Bar)
{
  pwned::progress::Progress p(10);
  p.ticks= 5;
  p.elapsed_milliseconds= 10000;
  std::string bar= p.bar();
  EXPECT_EQ(p.total_bar_width/ 2, std::count(bar.begin(), bar.end(), '='));
}

TEST(PwnedProgress, Print)
{
  std::ostringstream os;
  pwned::progress::Progress p(10, os);
  p.ticks= 5;
  p.elapsed_milliseconds= 10000;
  p.print();
  std::string output= os.str();
  EXPECT_NE(output.find("(00:00:10.000)"), std::string::npos);
  EXPECT_NE(output.find("(%  50)"), std::string::npos);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

