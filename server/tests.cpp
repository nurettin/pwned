#include <cassert>
#include <cstdio>
#include <iostream>
#include <gtest/gtest.h>
#include "server.hpp"
#include "../curl/curl.hpp"

using pwned::server::Router;
using pwned::server::Server;
struct ServerEnv: ::testing::Environment 
{
  static Router* r;
  static Server* s;
  void SetUp() 
  { 
    r= new Router;
    r-> add("GET_/users", [](mg_event*, Router::Params const &) {
      return "called_get_users";
    });
    r-> add("GET_/user/:id", [](mg_event*, Router::Params const &p) {
      return "called_get_user_"+ p.at("id");
    });
    r-> add("GET_/user/:id/post/:post_id", [](mg_event*, Router::Params const &p) {
      return "called_get_user_"+ p.at("id")+ "_post_"+ p.at("post_id");
    });

    s= new Server("8080");
    s-> get("/users", [](mg_event*, Router::Params const &) {
      return Server::response("called_get_users");
    });
    s-> get("/user/:id", [](mg_event*, Router::Params const &p) {
      return Server::response("called_get_user_"+ p.at("id"));
    });
    s-> get("/user/:id/post/:post_id", [](mg_event*, Router::Params const &p) {
      return Server::response("called_get_user_"+ p.at("id")+ "_post_"+ p.at("post_id"));
    });
  }
  void TearDown() 
  { 
    delete r;
    delete s;
  }
};

Router* ServerEnv::r(0);
Server* ServerEnv::s(0);

TEST(PwnedServerRouter, RouterMatch) 
{
  mg_event* event;
  auto match= ServerEnv::r-> match("GET_/users");
  ASSERT_TRUE(match);
  EXPECT_EQ(match-> first(event, match-> second), "called_get_users");
  match= ServerEnv::r-> match("GET_/user/42");
  ASSERT_TRUE(match);
  EXPECT_EQ(match-> first(event, match-> second), "called_get_user_42");
  match= ServerEnv::r-> match("GET_/user/11/post/542");
  ASSERT_TRUE(match);
  EXPECT_EQ(match-> first(event, match-> second), "called_get_user_11_post_542");
}

TEST(PwnedServer, ServerMatch)
{
  EXPECT_EQ(pwned::curl::open("http://localhost:8080/users"), "called_get_users");
  EXPECT_EQ(pwned::curl::open("http://localhost:8080/user/1"), "called_get_user_1");
  EXPECT_EQ(pwned::curl::open("http://localhost:8080/user/1/post/1"), "called_get_user_1_post_1");
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new ServerEnv);
  return RUN_ALL_TESTS();
}

