#include <iostream>
#include <map>
#include <boost/any.hpp>
#include <curl/curl.h>

typedef std::map<std::string, boost::any> params_t;

void open(params_t params)
{
  if(boost::any_cast<bool>(params["session"]))
    std::cout<< "session started!";
  curl_easy_cleanup(boost::any_cast<CURL*>(params["curl"]));
}

int main()
{
  open({{ "curl", curl_easy_init() }, { "session", true }});
}

