#include <cstdlib>
#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include "leveldb.hpp"

struct LeveldbEnv: ::testing::Environment 
{
  static pwned::leveldb::DB* db;

  void SetUp() { db= new pwned::leveldb::DB("data"); }
  void TearDown() { delete db; }
};

pwned::leveldb::DB* LeveldbEnv::db(0);

TEST(PwnedLeveldb, Empty)
{
  EXPECT_TRUE(LeveldbEnv::db-> empty());
  LeveldbEnv::db-> put("123", "abc");
  EXPECT_FALSE(LeveldbEnv::db-> empty());
  LeveldbEnv::db-> remove("123");
  EXPECT_TRUE(LeveldbEnv::db-> empty());
}

TEST(PwnedLeveldb, Put)
{
  LeveldbEnv::db-> put("1", "hello world");
}

TEST(PwnedLeveldb, Get)
{
  auto result= LeveldbEnv::db-> get("1");
  ASSERT_NE(result, boost::none);
  EXPECT_EQ(*result, "hello world");
}

TEST(PwnedLeveldb, Remove)
{
  LeveldbEnv::db-> remove("1");
}

TEST(PwnedLeveldb, ReverseEach)
{
  LeveldbEnv::db-> put("20130913114800", "abc");
  LeveldbEnv::db-> put("20130913114805", "def");
  LeveldbEnv::db-> put("20130913114900", "wtf");
  std::vector<std::string> data;
  LeveldbEnv::db-> reverse_each("20130913115000", "20130913114805", [&](std::string const &, std::string const &v) {
    data.push_back(v);
  });
  EXPECT_EQ(data, (std::vector<std::string> { "wtf", "def" }));
  LeveldbEnv::db-> reverse_each("20130913114804", "20130913114759", [&](std::string const &, std::string const &v) {
    data.push_back(v);
  });
  EXPECT_EQ(data, (std::vector<std::string> { "wtf", "def", "abc" }));
  LeveldbEnv::db-> remove("20130913114800");
  LeveldbEnv::db-> remove("20130913114805");
  LeveldbEnv::db-> remove("20130913114900");
}

TEST(PwnedLeveldb, Each)
{
  LeveldbEnv::db-> put("20130913114800", "abc");
  LeveldbEnv::db-> put("20130913114805", "def");
  LeveldbEnv::db-> put("20130913114900", "wtf");
  std::vector<std::string> data;
  LeveldbEnv::db-> each("20130913114800", "20130913114859", [&](std::string const &, std::string const &v) {
    data.push_back(v);
  });
  EXPECT_EQ(data, (std::vector<std::string> { "abc", "def" }));
  LeveldbEnv::db-> each("20130913114859", "20130913114900", [&](std::string const &, std::string const &v) {
    data.push_back(v);
  });
  EXPECT_EQ(data, (std::vector<std::string> { "abc", "def", "wtf" }));
  LeveldbEnv::db-> remove("20130913114800");
  LeveldbEnv::db-> remove("20130913114805");
  LeveldbEnv::db-> remove("20130913114900");
}

TEST(PwnedLeveldb, Batch)
{
  LeveldbEnv::db-> batch([](::leveldb::WriteBatch &batch) {
    batch.Put("20130913114800", "abc");
    batch.Put("20130913114805", "def");
    batch.Put("20130913114900", "wtf");
  });
  int count= 0;
  LeveldbEnv::db-> each("20130913114800", "20130913115000", [&](std::string const &, std::string const &) {
    ++ count;
  });
  EXPECT_EQ(count, 3);
  LeveldbEnv::db-> remove("20130913114800");
  LeveldbEnv::db-> remove("20130913114805");
  LeveldbEnv::db-> remove("20130913114900");
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new LeveldbEnv);
  return RUN_ALL_TESTS();
}

