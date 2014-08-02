#ifndef TIME_HPP
#define TIME_HPP

#include <string>
#include <ctime>
#include <stdexcept>

namespace time_util {

std::time_t ymd_to_time_t(std::string const &str)
{
  std::tm t= {};
  auto ret= strptime(str.c_str(), "%Y-%m-%d", &t);
  if(ret== 0)
    throw std::runtime_error("Error parsing time: "+ str);
  return std::mktime(&t);
}

std::string time_t_to_ymd(std::time_t t)
{
  std::tm* p_tm= localtime(&t);
  char buf[42]{};
  auto ret= strftime(buf, sizeof(buf), "%Y-%m-%d", p_tm);
  if(ret== 0)
    throw std::runtime_error("Error formatting time: "+ std::to_string(static_cast<unsigned>(t)));
  return buf;
}

} // time_util

#ifdef TIME_HPP_TEST

#include <cassert>

int main()
{
  assert( time_util::time_t_to_ymd(time_util::ymd_to_time_t("2014-07-20"))== "2014-07-20" );
}

#endif

#endif

