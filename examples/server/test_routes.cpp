#include <iostream>
#include <stdexcept>
#include <memory>
#include <re2/filtered_re2.h>

struct Router
{
  std::unique_ptr<re2::FilteredRE2> filter;
  std::vector<int> regex_indexes;
  std::vector<std::string> regexes;

  Router()
  : filter(new re2::FilteredRE2)
  {}

  void add(std::string const &regex)
  {
    filter.reset(new re2::FilteredRE2);
    if(regex_indexes.empty())
      regex_indexes.push_back(0);
    else
      regex_indexes.push_back(regex_indexes.back()+ 1);
    regexes.push_back(regex);

    for(std::size_t i= 0; i< regex_indexes.size(); ++ i)
      check(filter-> Add(regexes[i], RE2::Options(), &regex_indexes[i]));
    std::vector<std::string> strs;
    filter-> Compile(&strs);
  }

  int match(std::string const &url)
  {
    return filter-> FirstMatch(url, regex_indexes);
  }

  private:
  void check(RE2::ErrorCode const &code)
  {
    if(code== RE2::NoError)
      return;
    throw std::runtime_error("Error adding regex: "+ std::string(1, '0'+ code));
  }
};

int main()
{
  Router router;
  router.add("GET_/users");
  router.add("GET_/user/(?P<id>\\d+)");
  int match= router.match("GET_/user/1");
  if(match!= -1)
    std::cout<< match<< '\n';
  else
    std::cout<< "Regex not found!"<< '\n';
}
