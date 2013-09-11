#include <gtest/gtest.h>
#include "jsoncpp.hpp"

TEST(PwnedJson, ToString) {
  std::string str("{\"a\":{\"x\":1},\"b\":\"b\",\"c\":3.1415920}\n");
  pwned::jsoncpp::Json json(str);
  EXPECT_EQ(json.string(), str);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
