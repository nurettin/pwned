#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <gtest/gtest.h>
#include "debug.hpp"

TEST(PwnedDebug, EmptyVector)
{
  std::vector<int> v;
  std::ostringstream out;
  pwned::debug::p(v, out);
  EXPECT_EQ(out.str(), "[]\n");
}

TEST(PwnedDebug, Array)
{
  double a[]= { 0.5, 0.3, 0.2 };
  std::ostringstream out;
  pwned::debug::p(a, out);
  EXPECT_EQ(out.str(), "[0.5, 0.3, 0.2]\n");
}

TEST(PwnedDebug, Vector)
{
  std::vector<double> v{ 0.5, 0.3, 0.2 };
  std::ostringstream out;
  pwned::debug::p(v, out);
  EXPECT_EQ(out.str(), "[0.5, 0.3, 0.2]\n");
}

TEST(PwnedDebug, Map)
{
  std::map<std::string, int> m;
  m["a"]= 42;
  m["b"]= 43;
  std::ostringstream out;
  pwned::debug::p(m, out);
  EXPECT_EQ(out.str(), "[{\"a\": 42}, {\"b\": 43}]\n");
}

TEST(PwnedDebug, MapOfMap)
{
  std::map<std::string, std::map<std::string, int> > m;
  m["A"]["B"]= 42;
  m["A"]["C"]= 43;
  m["C"]["B"]= 44;
  std::ostringstream out;
  pwned::debug::p(m, out);
  EXPECT_EQ(out.str(), "[{\"A\": [{\"B\": 42}, {\"C\": 43}]}, {\"C\": [{\"B\": 44}]}]\n");
}

TEST(PwnedDebug, MapOfVector)
{
  std::map<std::string, std::vector<int>> mv;
  mv["a"]= {1, 2, 3};
  mv["b"]= {3, 4, 5};
  std::ostringstream out;
  pwned::debug::p(mv, out);
  EXPECT_EQ(out.str(), "[{\"a\": [1, 2, 3]}, {\"b\": [3, 4, 5]}]\n");
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

