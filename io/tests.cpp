#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <gtest/gtest.h>
#include "io.hpp"

TEST(PwnedIo, Array)
{
	double a[]= { 0.5, 0.3, 0.2 };
	std::ostringstream out;
	pwned::io::p(a, out);
	EXPECT_EQ(out.str(), "[0.5, 0.3, 0.2]");
}

TEST(PwnedIo, Vector)
{
	std::vector<double> v{0.5, 0.3, 0.2};
	std::ostringstream out;
	pwned::io::p(v, out);
	EXPECT_EQ(out.str(), "[0.5, 0.3, 0.2]\n");
}

TEST(PwnedIo, Map)
{
	std::map<std::string, int> m;
	m["a"]= 42;
	m["b"]= 44;
	std::ostringstream out;
	pwned::io::p(m, out);
	EXPECT_EQ(out.str(), "[{\"a\": 42}, {\"b\": 44}]\n");
}
/*
TEST(PwnedIo, MapOfMap)
{
	std::map<std::string, std::map<std::string, int> > m;
	m["Ankara"]["Istanbul"]= 42;
	m["Istanbul"]["Izmir"]= 44;
	std::ostringstream out;
	pwned::io::p(m, out);
	EXPECT_EQ(out.str(), "[(\"a\", 42), (\"b\", 44)]\n");
}
*/
int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

