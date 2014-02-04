#ifndef PWNED_JSONCPP_HPP
#define PWNED_JSONCPP_HPP

#include <string>
#include <stdexcept>
#include <json/json.h>

namespace pwned { namespace jsoncpp {

struct Json
{
  Json(){}

  Json(std::string const &json)
  {
    operator= (json);
  }

  std::string string() const
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
  
  template <typename T>
  ::Json::Value const &operator[](T const &t) const
  {
    return root[t];
  }
  
private:
  mutable ::Json::Value root;
  ::Json::Reader reader;
  mutable ::Json::FastWriter writer;
};

} }

#endif

