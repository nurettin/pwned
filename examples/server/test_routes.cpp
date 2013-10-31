#include <iostream>
#include <stdexcept>
#include <memory>
#include <boost/optional.hpp>
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

  boost::optional<std::string> matching_regex(std::string const &url)
  {
    int index= filter-> FirstMatch(url, regex_indexes);
    if(index== -1)
      return boost::none;
    return regexes[index];
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
  auto regex= router.matching_regex("GET_/user/1");
  if(!regex)
  {
    std::cout<< "Regex not found!"<< '\n';
    return EXIT_SUCCESS;
  }
  std::cout<< "match: "<< *regex<< '\n';
}
