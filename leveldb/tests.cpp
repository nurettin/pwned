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
  auto result= Env::db-> get("1");
  ASSERT_NE(result, boost::none);
  EXPECT_EQ(*result, "hello world");
}

TEST(PwnedLeveldb, ReverseEach)
{
  Env::db-> put("20130913114800", "abc");
  Env::db-> put("20130913114805", "def");
  Env::db-> put("20130913114900", "wtf");
  std::vector<std::string> data;
  Env::db-> reverse_each("20130913115000", "20130913114805", [&](std::string const &, std::string const &v) {
    data.push_back(v);
  });
  EXPECT_EQ(data, (std::vector<std::string> { "wtf", "def" }));
  Env::db-> reverse_each("20130913114804", "20130913114759", [&](std::string const &, std::string const &v) {
    data.push_back(v);
  });
  EXPECT_EQ(data, (std::vector<std::string> { "wtf", "def", "abc" }));
  Env::db-> remove("20130913114800");
  Env::db-> remove("20130913114805");
  Env::db-> remove("20130913114900");
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
  EXPECT_EQ(data, (std::vector<std::string> { "abc", "def" }));
  Env::db-> each("20130913114859", "20130913114900", [&](std::string const &, std::string const &v) {
    data.push_back(v);
  });
  EXPECT_EQ(data, (std::vector<std::string> { "abc", "def", "wtf" }));
  Env::db-> remove("20130913114800");
  Env::db-> remove("20130913114805");
  Env::db-> remove("20130913114900");
}

TEST(PwnedLeveldb, Batch)
{
  Env::db-> batch([](::leveldb::WriteBatch &batch) {
    batch.Put("20130913114800", "abc");
    batch.Put("20130913114805", "def");
    batch.Put("20130913114900", "wtf");
  });
  int count= 0;
  Env::db-> each("20130913114800", "20130913115000", [&](std::string const &, std::string const &) {
    ++ count;
  });
  EXPECT_EQ(count, 3);
  Env::db-> remove("20130913114800");
  Env::db-> remove("20130913114805");
  Env::db-> remove("20130913114900");
}

TEST(PwnedLeveldb, Empty)
{
  EXPECT_TRUE(Env::db-> empty());
  Env::db-> put("123", "abc");
  EXPECT_FALSE(Env::db-> empty());
  Env::db-> remove("123");
  EXPECT_TRUE(Env::db-> empty());
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new Env);
  return RUN_ALL_TESTS();
}

