#include <cstdlib>
#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include "active_proto.hpp"
#include "person.pb.h"

using namespace pwned::active_proto;

TEST(PwnedActiveProto, Count)
{
  Active<Person> person;
  EXPECT_EQ(person.count(), 0);
}

TEST(PwnedActiveProto, Insert)
{
  Active<Person> person;
  person.get().set_name("nurettin");
  person.save();
  EXPECT_EQ(person.count(), 1);
}

TEST(PwnedActiveProto, Update)
{
  Active<Person> person, newer;
  person.get().set_email("me@nurett.in");
  person.save();
  newer.load(2);
  EXPECT_EQ(newer.get().email(), "me@nurett.in");
}

TEST(PwnedActiveProto, All)
{
  Active<Person> person;
  auto people= person.all();
  EXPECT_EQ(people.size(), 2);
}

TEST(PwnedActiveProto, Delete)
{
  Active<Person> person;
  person.load(1);
  person.destroy();
  EXPECT_EQ(person.count(), 1);
}

TEST(PwnedActiveProto, DeleteAll)
{
  Active<Person> person;
  person.delete_all();
  EXPECT_EQ(person.count(), 0);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

