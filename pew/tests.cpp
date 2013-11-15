#include <gtest/gtest.h>
#include "pew.hpp"

TEST(PwnedPew, bench) 
{
  std::ostringstream out;
  pwned::pew::Pew pew;
  pew.bench("1st", []{ for(int n= 100000; n--;); });
  pew.bench("2nd", []{ for(int n= 1000000; n--;); });
  pew.bench("3rd", []{ for(int n= 10000000; n--;); });
  pew.run(4, out);
  std::string report= out.str();
  EXPECT_TRUE(report.find("1. 1st")!= std::string::npos);
  EXPECT_TRUE(report.find("2. 2nd")!= std::string::npos);
  EXPECT_TRUE(report.find("3. 3rd")!= std::string::npos);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
