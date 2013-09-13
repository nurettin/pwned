#include <cstdlib>
#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include "leveldb.hpp"

struct Env: ::testing::Environment 
{
  static pwned::leveldb::DB* db;

  void SetUp() { db= new pwned::leveldb::DB("data"); }
  void TearDown() { delete db; }
};

pwned::leveldb::DB* Env::db(0);

TEST(PwnedLeveldb, Remove)
{
  Env::db-> remove("1");
}

TEST(PwnedLeveldb, Put)
{
  Env::db-> put("1", "hello world");
}

TEST(PwnedLeveldb, Get)
{
  EXPECT_EQ(Env::db-> get("1"), "hello world");
}

TEST(PwnedLeveldb, Each)
{
  Env::db-> put("20130913114800", "abc");
  Env::db-> put("20130913114805", "def");
  Env::db-> put("20130913114900", "wtf");
  std::vector<std::string> data;
  Env::db-> each("20130913114800", "20130913114859", [&](std::string const &, std::string const &v) {
    data.push_back(v);
  });
  Env::db-> each("20130913114859", "20130913115000", [&](std::string const &, std::string const &v) {
    data.push_back(v);
  });
  Env::db-> remove("20130913114800");
  Env::db-> remove("20130913114805");
  Env::db-> remove("20130913114900");
  EXPECT_EQ(data, (std::vector<std::string> { "abc", "def", "wtf" }));
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new Env);
  return RUN_ALL_TESTS();
}

