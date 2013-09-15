#ifndef PWNED_FORMICATOR_HPP
#define PWNED_FORMICATOR_HPP

#include <map>
#include <string>
#include <htmlcxx/html/ParserDom.h>
#include <hcxselect.h>
#include "curl.hpp"

namespace pwned{ namespace curl {

struct Formicator
{
  typedef std::map<std::string, std::string> Params;
  
  CURL* curl;
  std::string user_agent;
  std::string page;

  Formicator(std::string const &user_agent= "Mozilla/4.0")
  : curl(curl_easy_init())
  , user_agent(user_agent)
  {
    check(curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str()));
    check(curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1));
    check(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1));
    check(curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""));
  }
  
  void reset_session()
  {
    curl_easy_cleanup(curl);
    curl= curl_easy_init();
  }

  ~Formicator()
  {
    curl_easy_cleanup(curl);
  }

  static std::string params_to_string(Params const &params)
  {
    std::string data;
    Params::const_iterator pb= params.begin(), pe= params.end();
    -- pe;
    for(; pb!= pe; ++ pb)
      data+= pb-> first+ "="+ pb-> second+ "&";
    data+= pb-> first+ "="+ pb-> second;
    return data;
  }

  void open(std::string const &uri)
  {
    page= "";
    pwned::curl::open(uri, page, curl, true);
  }

  void post(std::string const &url, Params const &params)
  {
    page= "";
    std::string params_string= params_to_string(params);
    std::vector<char> error_buffer(CURL_ERROR_SIZE);

    check(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params_string.c_str()));
    check(curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &error_buffer[0]));
    check(curl_easy_setopt(curl, CURLOPT_URL, url.c_str()));
    set_output_container(curl, page);
    
    int ok= curl_easy_perform(curl);
    if(ok!= 0)
      throw std::runtime_error(&error_buffer[0]);
  }

  // http://httpbin.org/
  // http://www.useragentstring.com/pages/Browserlist/
};

} } // pwned curl

#endif

