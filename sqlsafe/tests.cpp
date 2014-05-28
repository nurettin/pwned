#include <gtest/gtest.h>
#include "sqlsafe.hpp"

using namespace pwned::sqlsafe;

struct SqlSafeEnv: ::testing::Environment 
{
  static db_type db;

  void SetUp() 
  { 
    boost::log::core::get()-> set_logging_enabled(false);
    SqlSafeEnv::db= make_db(":memory:"); 
    execute(SqlSafeEnv::db, "create table wtf(id text)");
  }
  void TearDown() {}
};

db_type SqlSafeEnv::db;

TEST(PwnedSqlSafe, WriteScoped)
{
  auto insert= make_stmt(SqlSafeEnv::db, "insert into wtf(id) values(?)");
  for(int id= 1; id< 100; id+= 10)
    ASSERT_NO_THROW( execute(SqlSafeEnv::db, insert, std::to_string(id)) );
}

TEST(PwnedLeveldb, Read)
{
  auto select= make_stmt(SqlSafeEnv::db, "select id from wtf");
  std::tuple<char const*> row;
  int id= -9;
  while(get(SqlSafeEnv::db, select, row))
    ASSERT_EQ(std::to_string(id+= 10), std::get<0>(row));
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new SqlSafeEnv);
  return RUN_ALL_TESTS();
}

