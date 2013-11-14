#include <cassert>
#include <cstdio>
#include <iostream>
#include <gtest/gtest.h>
#include "server.hpp"
#include "../curl/curl.hpp"

using namespace pwned::server;
using namespace pwned::curl;
struct ServerEnv: ::testing::Environment 
{
  static Router* r;
  static Server* s;
  void SetUp() 
  { 
    r= new Router;
    r-> add("GET_/users", [](mg_event*, Params const &) {
      return "called_get_users";
    });
    r-> add("GET_/user/:id", [](mg_event*, Params const &p) {
      return "called_get_user_"+ p.at("id");
    });
    r-> add("GET_/user/:id/post/:post_id", [](mg_event*, Params const &p) {
      return "called_get_user_"+ p.at("id")+ "_post_"+ p.at("post_id");
    });

    s= new Server;
    s-> Get("/", [](mg_event*, Params const &) {
      return Server::response("called_root");
    });
    s-> Get("/users", [](mg_event*, Params const &) {
      return Server::response("called_get_users");
    });
    s-> Get("/user/:id", [](mg_event*, Params const &p) {
      return Server::response("called_get_user_"+ p.at("id"));
    });
    s-> Get("/user/:id/post/:post_id", [](mg_event*, Params const &p) {
      return Server::response("called_get_user_"+ p.at("id")+ "_post_"+ p.at("post_id"));
    });
    s-> Post("/user/create", [](mg_event*, Params const &p) {
      return Server::response("username: "+ p.at("username")+ " password: "+ p.at("password"));
    });
    s-> Get("/search", [](mg_event*, Params const &p) {
      return Server::response("query: "+ p.at("q")+ ", topic: "+ p.at("topic"));
    });
    s-> Get("/zip", [](mg_event*, Params const &) {
      return Server::response_gzip("hi this is dog");
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

TEST(PwnedServer, Static)
{
  EXPECT_EQ(open("http://localhost:8080"), "called_root");
  EXPECT_EQ(open("http://localhost:8080/users"), "called_get_users");
}

TEST(PwnedServer, Rest)
{
  EXPECT_EQ(open("http://localhost:8080/user/1"), "called_get_user_1");
  EXPECT_EQ(open("http://localhost:8080/user/1/post/1"), "called_get_user_1_post_1");
}

TEST(PwnedServer, Post)
{
  std::string result;
  post("http://localhost:8080/user/create", result, "username=onur&password=123456");
  EXPECT_EQ(result, "username: onur password: 123456");
}

TEST(PwnedServer, Get)
{
  EXPECT_EQ(open("http://localhost:8080/search?q=C++&topic=C++11"), "query: C++, topic: C++11");
}

TEST(PwnedServer, Https)
{
  auto curl= curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  std::string result;
  get("https://localhost:4343/search?q=C++&topic=C++11", result, curl);
  EXPECT_EQ(result, "query: C++, topic: C++11");
}

TEST(PwnedServer, Gzip)
{
  auto curl= curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
  std::string result;
  get("http://localhost:8080/zip", result, curl);
  EXPECT_EQ(result, "hi this is dog");
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new ServerEnv);
  return RUN_ALL_TESTS();
}

