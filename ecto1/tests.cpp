#include <fstream>
#include <gtest/gtest.h>
#include "ecto1.hpp"

using namespace pwned::curl;
struct Ecto1Env: ::testing::Environment 
{
  static pwned::ecto1::Ecto1* d;
  void SetUp() 
  {
    // check for command processor
    ASSERT_TRUE(std::system(0));
    std::system("./start_phantom_16666.sh"); 
    ASSERT_NO_THROW(d= new pwned::ecto1::Ecto1("http://localhost:16666"));
  }
  void TearDown() 
  {
    delete d;
    std::system("./stop_phantom_16666.sh");
    std::remove("shot.html");
  }
};

pwned::ecto1::Ecto1* Ecto1Env::d(0);

TEST(PwnedEcto1, Get) 
{
  ASSERT_NO_THROW(Ecto1Env::d-> get("https://google.com"));
}

TEST(PwnedEcto1, SetValueByName)
{
  ASSERT_NO_THROW(Ecto1Env::d-> set_value_by_name("q", "ghostbusters"));
}

TEST(PwnedEcto1, ClickById)
{
  ASSERT_NO_THROW(Ecto1Env::d-> click_by_id("gbqfb"));
}

TEST(PwnedEcto1, Screenshot)
{
  std::ofstream shot("shot.html");
  ASSERT_NO_THROW(shot<< ("<img src='"+ Ecto1Env::d-> screenshot_mime())<< "' />");
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new Ecto1Env);
  return RUN_ALL_TESTS();
}

