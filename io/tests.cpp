#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <gtest/gtest.h>
#include "io.hpp"

TEST(PwnedIo, EmptyVector)
{
  std::vector<int> v;
  std::ostringstream out;
  pwned::io::p(v, out);
  EXPECT_EQ(out.str(), "[]\n");
}

TEST(PwnedIo, Array)
{
  double a[]= { 0.5, 0.3, 0.2 };
  std::ostringstream out;
  pwned::io::p(a, out);
  EXPECT_EQ(out.str(), "[0.5, 0.3, 0.2]\n");
}

TEST(PwnedIo, Vector)
{
  std::vector<double> v{ 0.5, 0.3, 0.2 };
  std::ostringstream out;
  pwned::io::p(v, out);
  EXPECT_EQ(out.str(), "[0.5, 0.3, 0.2]\n");
}

TEST(PwnedIo, Map)
{
  std::map<std::string, int> m;
  m["a"]= 42;
  m["b"]= 43;
  std::ostringstream out;
  pwned::io::p(m, out);
  EXPECT_EQ(out.str(), "[{\"a\": 42}, {\"b\": 43}]\n");
}

TEST(PwnedIo, MapOfMap)
{
  std::map<std::string, std::map<std::string, int> > m;
  m["A"]["B"]= 42;
  m["A"]["C"]= 43;
  m["C"]["B"]= 44;
  std::ostringstream out;
  pwned::io::p(m, out);
  EXPECT_EQ(out.str(), "[{\"A\": [{\"B\": 42}, {\"C\": 43}]}, {\"C\": [{\"B\": 44}]}]\n");
}

TEST(PwnedIo, MapOfVector)
{
  std::map<std::string, std::vector<int>> mv;
  mv["a"]= {1, 2, 3};
  mv["b"]= {3, 4, 5};
  std::ostringstream out;
  pwned::io::p(mv, out);
  EXPECT_EQ(out.str(), "[{\"a\": [1, 2, 3]}, {\"b\": [3, 4, 5]}]\n");
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

