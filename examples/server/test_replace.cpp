#include <string>
#include <vector>
#include <iostream>
#include <re2/re2.h>

int main()
{
  std::string route("GET_/users/:id/posts/:post_id/comments");
  std::cout<< route<< '\n';
  re2::RE2 re("(:([^/]+))");
  re2::RE2::GlobalReplace(&route, re, "(?P<\\2>[^/])");
  std::cout<< route<< '\n';
}

