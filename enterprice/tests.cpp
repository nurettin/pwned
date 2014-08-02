#include <gtest/gtest.h>
#include "memory.hpp"
#include "time.hpp"
#include "mysql.hpp"
#include "candle_stream.hpp"
#include "multi_candle_stream.hpp"

using namespace pwned;

struct EnterpriceEnv: ::testing::Environment 
{
  static int gc_value;
  static void gc_test(int*){ gc_value= 1; }
  void SetUp() 
  {
  }
  void TearDown() {}
};

int EnterpriceEnv::gc_value(0);

TEST(PwnedEnterprice, gc)
{
  {
    int i(42);
    auto wtf= memory::gc(&i, EnterpriceEnv::gc_test);
  }
  ASSERT_EQ(EnterpriceEnv::gc_value, 1);
}

TEST(PwnedEnterprice, time_util)
{
  ASSERT_EQ(time_util::time_t_to_ymd(time_util::ymd_to_time_t("2014-07-20")), "2014-07-20");
}

#include <tuple>
#include <vector>

TEST(PwnedEnterprice, candle_stream)
{
  typedef std::vector<std::tuple<unsigned long, double, double, double, double, double>> data_t;
  data_t data;

  enterprice::CandleStream strm(5, [&](unsigned long ts, double o, double h, double l, double c, double v){
    data.push_back(std::make_tuple(ts, o, h, l, c, v));
  });

  strm.init_data(1000, 10, 1);
  strm.data(1003, 12, 2);
  strm.data(1007, 9, 1);
  strm.data(1011, 10, 3);
  strm.data(1021, 11, 4);
  strm.data(1022, 1, 4);
  strm.data(1023, 18, 4);
  strm.data(1024, 12, 4);
  strm.data(1026, 9, 4);
  strm.data(1036, 9, 4);

  ASSERT_EQ( data[0], data_t::value_type(1000, 10, 12, 10, 12, 3) );
  ASSERT_EQ( data[1], data_t::value_type(1005, 12, 9, 9, 9, 1) );
  ASSERT_EQ( data[2], data_t::value_type(1010, 9, 10, 10, 10, 3) );
  ASSERT_EQ( data[3], data_t::value_type(1015, 10, 10, 10, 10, 0) );
  ASSERT_EQ( data[4], data_t::value_type(1020, 10, 18, 1, 12, 16) );
  ASSERT_EQ( data[5], data_t::value_type(1025, 12, 9, 9, 9, 4) );
  ASSERT_EQ( data[6], data_t::value_type(1030, 9, 9, 9, 9, 0) );
}

TEST(PwnedEnterprice, multi_candle_stream)
{
  typedef std::vector<std::tuple<std::string, unsigned long, double, double, double, double, double>> data_t;
  data_t data;
  
  enterprice::MultiCandleStream streams(5, [&](std::string const &code, unsigned long ts, double o, double h, double l, double c, double v){
    data.push_back(std::make_tuple(code, ts, o, h, l, c, v));
  });

  streams.data("OMG", 1000, 10, 1);
  streams.data("LOL", 1001, 12, 1);
  streams.data("LOL", 1005, 10, 1);
  streams.data("LOL", 1006, 9, 1);
  streams.data("OMG", 1006, 10, 1);
  
  ASSERT_EQ( data[0], data_t::value_type("LOL", 1001, 12, 12, 10, 10, 2) );
  ASSERT_EQ( data[1], data_t::value_type("OMG", 1000, 10, 10, 10, 10, 1) );
}

TEST(PwnedEnterprice, mysql)
{
  auto db= mysql::connect("ensembldb.ensembl.org", "anonymous", "", "felis_catus_core_75_62");
  int count= 0;
  mysql::query(db.get(), "select count(1) from meta", [&](MYSQL_ROW row){
    count= std::stoi(row[0]);
  });
  ASSERT_TRUE(count> 0);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new EnterpriceEnv);
  return RUN_ALL_TESTS();
}

