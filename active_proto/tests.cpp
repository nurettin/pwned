#include <cstdlib>
#include <cassert>
#include <iostream>
#include <gtest/gtest.h>
#include "active_proto.hpp"
#include "db.pb.h"

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
  person.load(1);
  person.get().set_email("me@nurett.in");
  person.save();
  newer.load(1);
  EXPECT_EQ(newer.get().email(), "me@nurett.in");
}

TEST(PwnedActiveProto, All)
{
  Active<Person> person;
  auto people= person.all();
  EXPECT_EQ(people.size(), 1);
}

TEST(PwnedActiveProto, Delete)
{
  Active<Person> person;
  person.load(1);
  person.destroy();
  EXPECT_EQ(person.count(), 0);
}

TEST(PwnedActiveProto, DeleteAll)
{
  Active<Person> person1, person2;
  person1.get().set_name("name");
  person1.save();
  person2.get().set_email("email");
  person2.save();
  person1.delete_all();
  EXPECT_EQ(person1.count(), 0);
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

