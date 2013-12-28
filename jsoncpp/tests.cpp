#include <gtest/gtest.h>
#include "jsoncpp.hpp"

TEST(PwnedJson, ToString) 
{
  std::string str("{\"a\":{\"x\":1},\"b\":\"b\",\"c\":3.1415920}\n");
  pwned::jsoncpp::Json json(str);
  EXPECT_EQ(json.string(), str);
}

TEST(PwnedJson, Error)
{
  EXPECT_THROW(pwned::jsoncpp::Json("{\"x\": x}"), std::runtime_error);
}

TEST(PwnedJson, Operator)
{
  pwned::jsoncpp::Json json;
  json["test"][0]["a"]= 1;
  json["test"][1]["b"]= 2;
  EXPECT_EQ(json.string(), "{\"test\":[{\"a\":1},{\"b\":2}]}\n");
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
