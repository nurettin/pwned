#ifndef PWNED_CURL_HPP
#define PWNED_CURL_HPP

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
#include <curl/curl.h>

namespace pwned { namespace curl {

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
  // insert will work on any sequence including std::string
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

template <typename containerT>
CURL* open(std::string const &url, containerT &c, CURL* curl= curl_easy_init(), bool session= true)
{
  if(curl== 0)
    curl= curl_easy_init();
  check(curl_easy_setopt(curl, CURLOPT_HTTPGET, 1));
  check(curl_easy_setopt(curl, CURLOPT_URL, url.c_str()));
  pwned::curl::set_output_container(curl, c);
  std::vector<char> error_buffer(CURL_ERROR_SIZE);
  check(curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &error_buffer[0]));
  int ok= curl_easy_perform(curl);
  if(ok!= 0)
    throw std::runtime_error(&error_buffer[0]);
  if(session)
    return curl;
  curl_easy_cleanup(curl);
  return 0;
}

std::string open(std::string const &url)
{
  std::string result;
  open(url, result);
  return result;
}

} // curl
} // pwned

#endif

