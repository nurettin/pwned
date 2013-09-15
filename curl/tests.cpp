#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include "curl.hpp"
#include "formicator.hpp"

struct Env: ::testing::Environment 
{
  static pwned::curl::Formicator* f;
  void SetUp() 
  { 
    std::ofstream touch("touch.txt"); 
    touch<< "touch"<< std::flush;
    f= new pwned::curl::Formicator("http://httpbin.org");
  }
  void TearDown() 
  { 
    std::remove("touch.txt"); 
    delete f;
  }
};

pwned::curl::Formicator* Env::f(0);

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

TEST(PwnedCurl, ValidUrl) 
{
  std::string data= pwned::curl::open("http://httpbin.org/robots.txt");
  EXPECT_EQ(boost::algorithm::starts_with(data, "User-agent: *"), true);
}

TEST(PwnedCurlFormicator, Get)
{
  Env::f-> get("/html");
  EXPECT_EQ(boost::algorithm::starts_with(Env::f-> page, "<!DOCTYPE html>"), true);
}

TEST(PwnedCurlFormicator, Post)
{
  Env::f-> post("/post", { { "abc", "123" } });
  EXPECT_EQ(boost::algorithm::contains(Env::f-> page, "\"abc\": \"123\""), true);
  Env::f-> get("/cookies/set?session=sessionid123");
  Env::f-> get("/cookies");
  EXPECT_EQ(boost::algorithm::contains(Env::f-> page, "\"session\": \"sessionid123\""), true);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new Env);
  return RUN_ALL_TESTS();
}

