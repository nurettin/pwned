#include <vector>
#include <string>
#include <iostream>
#include <re2/re2.h>

int main()
{
  re2::RE2 re("(:[^/]+)");
  std::cout<< "regex ok: "<< re.ok()<< '\n';
  bool ok= re2::RE2::PartialMatch("GET_/user/:id", re.pattern(), (void*)0);
  std::cout<< "match: "<< ok<< '\n';
}

