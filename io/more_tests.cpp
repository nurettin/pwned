#include <vector>
#include <map>
#include "io.hpp"

int main()
{
  using pwned::io::operator<<;

  std::cout<< std::string("x")<< '\n';
  std::vector<int> v1{ 1, 2, 3 };
  std::cout<< v1<< '\n';
  std::vector<std::string> v2{ "ab", "bc", "cd" };
  std::cout<< v2<< '\n';
  std::vector<std::vector<std::string>> v3{ {"lol", "omg"}, {"wtf"} };
  std::cout<< v3<< '\n';
  std::map<std::string, std::string> m;
  m["a"]= "b";
  std::cout<< m<< '\n';
  std::map<std::string, std::map<std::string, int>> m2;
  m2["a"]["b"]= 41;
  m2["a"]["c"]= 42;
  m2["b"]["c"]= 43;
  std::cout<< m2<< '\n';
  int a[3]= { 1, 2, 3 };
  std::cout<< a<< '\n';
}

