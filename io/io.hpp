#ifndef PWNED_IO_HPP
#define PWNED_IO_HPP

#include <iostream>
#include <ostream>
#include <string>
#include <boost/range/iterator_range.hpp>

// TODO: recurse into container of containers
namespace pwned { namespace io {

template <typename T>
struct formatter
{
  T const &value;
  formatter(T const &value): value(value){}
  formatter(formatter<T> const &f): value(f.value){}
};

template <typename T>
formatter<T> make_formatter(T const &value){ return formatter<T>(value); }

template <typename T>
std::ostream &operator<< (std::ostream &o, formatter<T> const &f){ return o<< f.value; }

template <typename T>
std::ostream &operator<< (std::ostream &o, boost::iterator_range<T> const &r)
{
  o<< '[';
  typename boost::iterator_range<T>::const_iterator b= r.begin(), e= r.end(); 
  -- e;
  for(;b!= e; ++ b) o<< make_formatter(*b)<< ", ";
  return o<< make_formatter(*e)<< ']';
}

template <>
std::ostream &operator<< <std::string>(std::ostream &o, formatter<std::string> const &f)
{
  return o<< '"'<< f.value<< '"';
}

template <typename K, typename V>
std::ostream &operator<< (std::ostream &o, std::pair<K, V> const &p)
{
  return o<< '('<< make_formatter(p.first)<< ", "<< make_formatter(p.second)<< ')';
}

template <typename T>
std::ostream &p(T const &c, std::ostream &o= std::cout)
{
  return o<< boost::make_iterator_range(c)<< '\n';
}

} // io
} // pwned

#endif

