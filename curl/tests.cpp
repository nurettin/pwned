#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
using boost::filesystem::current_path;
#include <gtest/gtest.h>
#include "curl.hpp"
using std::string;

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
    string result= pwned::curl::open("wtf");
  }
  catch(std::runtime_error &)
  {
    throws_on_invalid_url= true;
  }
  EXPECT_EQ(throws_on_invalid_url, true);
}

TEST(PwnedCurl, LocalFile) 
{
  EXPECT_EQ(pwned::curl::open("file://"+ (current_path()/ "touch.txt").string()), "touch");
}

TEST(PwnedCurl, ValidUrl) 
{
  EXPECT_EQ(boost::algorithm::starts_with(
  pwned::curl::open("http://duckduckgo.com/robots.txt"), "User-agent: *"), true);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new Env);
  return RUN_ALL_TESTS();
}

