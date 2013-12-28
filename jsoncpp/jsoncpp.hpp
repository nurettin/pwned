#ifndef PWNED_JSONCPP_HPP
#define PWNED_JSONCPP_HPP

#include <string>
#include <stdexcept>
#include <json/json.h>

namespace pwned { namespace jsoncpp {

struct Json
{
  ::Json::Value root;
  ::Json::Reader reader;
  ::Json::FastWriter writer;

  Json(){}

  Json(std::string const &json)
  {
    operator= (json);
  }

  std::string string()
  {
    return writer.write(root);	
  }

  void operator= (std::string const &json)
  {
    bool ok= reader.parse(json, root);
    if(!ok)
      throw std::runtime_error(reader.getFormattedErrorMessages());
  }

  template <typename T>
  ::Json::Value &operator[](T const &t)
  {
    return root[t];
  }
};

} }

#endif

