#include <string>
#include <boost/variant.hpp>
#include <gtest/gtest.h>
#include "parameters.hpp"

struct Storage
{
  typedef pwned::parameters::Parameters<boost::variant<int, double, std::string>> ParameterType;
  ParameterType params;

  Storage(ParameterType::data_t const &data= {})
  : params(default_params().impose(data))
  {}

  static ParameterType default_params()
  {
    return ParameterType({
      { "int", 1 },
      { "double", 3.14 },
      { "string", "hello" }
    });
  }
};

TEST(PwnedParameters, Default)
{
  Storage store;
  EXPECT_EQ(1, boost::get<int>(store.params.data["int"]));
  EXPECT_FLOAT_EQ(3.14, boost::get<double>(store.params.data["double"]));
  EXPECT_EQ("hello", boost::get<std::string>(store.params.data["string"]));
}

TEST(PwnedParameters, Impose)
{
  Storage store({ { "int", 42 } });
  EXPECT_EQ(42, boost::get<int>(store.params.data["int"]));
  EXPECT_FLOAT_EQ(3.14, boost::get<double>(store.params.data["double"]));
  EXPECT_EQ("hello", boost::get<std::string>(store.params.data["string"]));
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

