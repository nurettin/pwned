#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include "curl.hpp"

struct Env: ::testing::Environment 
{
  void SetUp() { std::ofstream touch("touch.txt"); touch<< "touch"<< std::flush; }
  void TearDown() { std::remove("touch.txt"); }
};

TEST(PwnedCurl, InvalidUrl) 
{
  bool throws_on_invalid_url= false;
  try
  {
    std::string result= pwned::curl::open("wtf");
  }
  catch(std::runtime_error &)
  {
    throws_on_invalid_url= true;
  }
  EXPECT_EQ(throws_on_invalid_url, true);
}

TEST(PwnedCurl, LocalFile) 
{
  EXPECT_EQ(pwned::curl::open("file://"+ (boost::filesystem::current_path()/ "touch.txt").string()), "touch");
}
#include <iostream>
TEST(PwnedCurl, ValidUrl) 
{
  std::string data= pwned::curl::open("http://duckduckgo.com/robots.txt");
  std::cout<< data<< '\n';
  EXPECT_EQ(boost::algorithm::starts_with(
  data, "User-agent: *"), true);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new Env);
  return RUN_ALL_TESTS();
}

