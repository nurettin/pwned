#ifndef PWNED_FORMICATOR_HPP
#define PWNED_FORMICATOR_HPP

#include "curl.hpp"

// http://www.mostthingsweb.com/2013/02/parsing-html-with-c/
// http://stackoverflow.com/questions/2689513/handling-responses-in-libcurl
// http://www.hackthissite.org/articles/read/1078
struct Formicator
{
  CURL* curl;

  Formicator()
  : curl(curl_easy_init())
  {}

  void open(std::string const &uri)
  {
    std::string result;
    pwned::curl::open(uri, result, curl, true);
  }

  void post(std::map<std::string, std::string> const &params)
  {

  }

  void clean_session()
  {
    curl_easy_cleanup(curl);
  }

  ~Formicator()
  {
    clean_session();
  }
};

#endif

