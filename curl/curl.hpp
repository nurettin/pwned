#ifndef PWNED_CURL_HPP
#define PWNED_CURL_HPP

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
#include <curl/curl.h>

namespace pwned { namespace curl {

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
  curl_easy_setopt(c, CURLOPT_WRITEDATA, &s);
  std::size_t(*f)(void*, std::size_t, std::size_t, void*)= output_to_container<containerT>;
  curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, f);
}

template <typename containerT>
void open(std::string const &url, containerT &c)
{
  CURL* curl= curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  pwned::curl::set_output_container(curl, c);
  std::vector<char> error_buffer(CURL_ERROR_SIZE);
  curl_easy_setopt(curl, CURLOPT_HEADER, 1);
  curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &error_buffer[0]);
  int ok= curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  if(ok!= 0)
    throw std::runtime_error(&error_buffer[0]);
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

