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

  Json(std::string const &json)
  {
    operator= (json);
  }

  std::string Json::to_string()
  {
    return writer.write(root);	
  }

  void Json::operator= (std::string const &json)
  {
    bool ok= reader.parse(json, root);
    if(!ok)
      throw std::runtime_error(reader.getFormattedErrorMessages());
  }
};

} }

#endif

