#ifndef PWNED_CURL_HPP
#define PWNED_CURL_HPP

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <curl/curl.h>

namespace pwned { namespace curl {

typedef std::map<std::string, std::string> Params;

void check(CURLcode code)
{
  if(code!= CURLE_OK)
    throw std::runtime_error("Unrecognized option.");
}

template <typename containerT>
std::size_t output_to_container(void* data, std::size_t size, std::size_t nmemb, void* input)
{
  containerT* con= static_cast<containerT*>(input);
  char* cstr= static_cast<char*>(data);
  std::size_t len= size* nmemb;
  try { con-> insert(con-> end(), cstr, cstr+ len); }
  catch(...) { return CURLE_WRITE_ERROR; }
  return len;
}

template <typename containerT>
void set_output_container(CURL* c, containerT &s)
{
  check(curl_easy_setopt(c, CURLOPT_WRITEDATA, &s));
  std::size_t(*f)(void*, std::size_t, std::size_t, void*)= output_to_container<containerT>;
  check(curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, f));
}

std::string params_to_string(Params const &params)
{
  std::string data;
  Params::const_iterator pb= params.begin(), pe= params.end();
  -- pe;
  for(; pb!= pe; ++ pb)
    data+= pb-> first+ "="+ pb-> second+ "&";
  data+= pb-> first+ "="+ pb-> second;
  return data;
}

template <typename containerT>
CURL* perform(std::string const &url, containerT &c, CURL* curl, bool session, std::function<void()> block)
{
  if(curl== 0)
    curl= curl_easy_init();
  std::vector<char> error_buffer(CURL_ERROR_SIZE);
  block();
  check(curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &error_buffer[0]));
  check(curl_easy_setopt(curl, CURLOPT_URL, url.c_str()));
  set_output_container(curl, c);
  
  int ok= curl_easy_perform(curl);
  if(ok!= 0)
    throw std::runtime_error(&error_buffer[0]);
  if(session)
    return curl;
  curl_easy_cleanup(curl);
  return 0;
}

template <typename containerT>
CURL* get(std::string const &url, containerT &c, CURL* curl= curl_easy_init(), bool session= false)
{
  return perform(url, c, curl, session, [&](){
    check(curl_easy_setopt(curl, CURLOPT_HTTPGET, 1));
  });
}

template <typename containerT>
CURL* post(std::string const &url, containerT &c, char const* params_string, CURL* curl= curl_easy_init(), bool session= false)
{
  return perform(url, c, curl, session, [&](){
    check(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params_string));
  });
}

std::string open(std::string const &url)
{
  std::string result;
  get(url, result);
  return result;
}

} // curl
} // pwned

#endif

