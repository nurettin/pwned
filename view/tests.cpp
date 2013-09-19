#include <gtest/gtest.h>
#include "view.hpp"

using pwned::view::view;

TEST(PwnedView, View)
{
  std::vector<int> v{ 1, 2, 3 };
  int sum= view(v).each([](int &i){ i+= 1; }).sum(0);
  EXPECT_EQ(sum, 9);
}

TEST(PwnedView, ConstView)
{
  std::vector<int> v{ 1, 2, 3 };
  std::vector<int>::size_type vi= 0;
  view(std::vector<int>{ 1, 2, 3 }).each([&](int const &i){ v[vi]+= i; ++ vi; });
  int sum= view(v).sum(0);
  EXPECT_EQ(sum, 12);
}

TEST(PwnedView, CopyView)
{
  std::vector<int> v{ 1, 2, 3 };
  int sum= view(v).each([](int &i){ i+= 1; }).
    map([](int i){ return i* 2; }).
    each([](int &i){ return i+= 1; }).
    select([](int const &i){ return i< 8; }).
    sum(0);
  EXPECT_EQ(sum, 12);
}

TEST(PwnedView, RangeView)
{
  std::vector<int> v{ 1, 2, 3 };
  int sum= view(v.begin(), v.end()).select([](int const &i){ return i< 3; }).sum(0);
  EXPECT_EQ(sum, 3);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

