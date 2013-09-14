#ifndef PWNED_FORMICATOR_HPP
#define PWNED_FORMICATOR_HPP

#include <map>
#include <string>
#include <iostream>
#include "curl.hpp"

// http://www.mostthingsweb.com/2013/02/parsing-html-with-c/
// http://www.hackthissite.org/articles/read/1078
// http://hcxselect.sourceforge.net/
// https://github.com/jgehring/hcxselect

namespace pwned{ namespace curl {

struct Formicator
{
  typedef std::map<std::string, std::string> Params;
  CURL* curl;
  std::string page;

  Formicator()
  : curl(curl_easy_init())
  {
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/4.0");
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
  }

  void open(std::string const &uri)
  {
    page= "";
    pwned::curl::open(uri, page, curl, true);
  }

  void post(std::string const &url, Params const &params)
  {
    std::string data;
    Params::const_iterator pb= params.begin(), pe= params.end();
    -- pe;
    for(; pb!= pe; ++ pb)
      data+= pb-> first+ "="+ pb-> second+ "&";
    data+= pb-> first+ "="+ pb-> second;
    page= "";
    pwned::curl::set_output_container(curl, page);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    std::vector<char> error_buffer(CURL_ERROR_SIZE);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &error_buffer[0]);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    int ok= curl_easy_perform(curl);
    if(ok!= 0)
      throw std::runtime_error(&error_buffer[0]);
  }

  void print()
  {
    std::cout<< page<< '\n';
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

} } // pwned curl

#endif

