#ifndef PWNED_PARAMETERS_HPP
#define PWNED_PARAMETERS_HPP

#include <map>

namespace pwned { namespace parameters {

template <typename Variant>
struct Parameters
{
  typedef std::map<std::string, Variant> data_t;
  data_t data;
  
  Parameters(data_t const &data)
  : data(data)
  {}

  Parameters()= delete;
  Parameters(Parameters const &)= delete;
  Parameters(Parameters &&p)
  : data(p.data)
  {}

  Parameters &&impose(data_t const &other)
  {
    for(auto const &e: other)
      data[e.first]= e.second;
    return std::move(*this);
  }
};

} }

#endif

