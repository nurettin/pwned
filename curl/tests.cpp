#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include "curl.hpp"
#include "formicator.hpp"

struct CurlEnv: ::testing::Environment 
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

pwned::curl::Formicator* CurlEnv::f(0);

TEST(PwnedCurl, InvalidUrl) 
{
  EXPECT_THROW(pwned::curl::open("wtf"), std::runtime_error);
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
  CurlEnv::f-> get("/html");
  EXPECT_EQ(boost::algorithm::starts_with(CurlEnv::f-> page, "<!DOCTYPE html>"), true);
}

TEST(PwnedCurlFormicator, Post)
{
  CurlEnv::f-> post("/post", { { "abc", "123" }, { "def", "456" } });
  EXPECT_EQ(boost::algorithm::contains(CurlEnv::f-> page, "\"abc\": \"123\""), true);
  EXPECT_EQ(boost::algorithm::contains(CurlEnv::f-> page, "\"def\": \"456\""), true);
}

TEST(PwnedCurlFormicator, Cookies)
{
  CurlEnv::f-> get("/cookies/set?session=sessionid123");
  CurlEnv::f-> get("/cookies");
  EXPECT_EQ(boost::algorithm::contains(CurlEnv::f-> page, "\"session\": \"sessionid123\""), true);
}

TEST(PwnedCurlFormicator, Find)
{
  CurlEnv::f-> get("/html");
  auto h1= CurlEnv::f-> find("h1");
  ASSERT_TRUE(h1!= 0);
  ASSERT_TRUE(h1-> first_child!= 0);
  EXPECT_EQ(h1-> first_child-> data.text(), "Herman Melville - Moby-Dick");
}

TEST(PwnedCurlFormicator, Delete)
{
  CurlEnv::f-> delete_("/delete?data=42");
  EXPECT_EQ(boost::algorithm::contains(CurlEnv::f-> page, "data=42"), true);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new CurlEnv);
  return RUN_ALL_TESTS();
}

