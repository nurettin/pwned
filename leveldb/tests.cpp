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

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new Env);
  return RUN_ALL_TESTS();
}

