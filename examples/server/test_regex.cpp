#include <vector>
#include <string>
#include <iostream>
#include <re2/re2.h>

using namespace std;

std::map<std::string, std::string> extract_params(
  std::string const &uri
, re2::RE2 const &re)
{
  re2::StringPiece input(uri);
  int groupSize = re.NumberOfCapturingGroups();
  vector<re2::StringPiece> ws(groupSize);
  vector<re2::RE2::Arg> argv(groupSize);
  vector<re2::RE2::Arg*> args(groupSize);
  for (int i = 0; i < groupSize; ++i) {
    args[i] = &argv[i];
    argv[i] = &ws[i];
  }
  bool ok= re2::RE2::FullMatchN(input, re, &(args[0]), groupSize);
  if(!ok)
    throw std::runtime_error("Error matching: "+ uri+ " to "+ re.pattern());
  std::map<std::string, std::string> result;
  for(auto &cgn: re.CapturingGroupNames())
    result[cgn.second]= ws[cgn.first- 1].ToString();
  return result;
}

int main()
{
  re2::RE2 re("GET_/users/(?P<id>[^/]+)/posts/(?P<post_id>[^/]+)");
  auto result= extract_params("GET_/users/1/posts/3", re);
  for(auto &r: result)
    cout<< r.first<< " = "<< r.second<< '\n';
}

