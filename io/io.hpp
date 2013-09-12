#ifndef PWNED_IO_HPP
#define PWNED_IO_HPP

#include <iostream>
#include <ostream>
#include <string>

namespace pwned { namespace io {

template <typename T>
void puts(std::ostream &o, T const &t) { o<< t; }

void puts(std::ostream &o, std::string const &s){ o<< '"'<< s<< '"'; }

template <typename K, typename V>
void puts(std::ostream &o, std::pair<K, V> const &p)
{
  o<< '{'; puts(o, p.first);
  o<< ": "; puts(o, p.second);
  o<< '}';
}

template <typename C, typename I= typename C::const_iterator>
typename std::enable_if<!std::is_same<C, std::string>::value, std::ostream &>::type
operator<< (std::ostream &o, C const &c)
{
  o<< '[';
  if(c.empty()) return o<< ']';
  I b= c.begin(), e= c.end(); -- e;
  for(; b!= e; ++ b)
  {
    puts(o, *b);
    o<< ", ";
  }
  puts(o, *b);
  return o<< ']';
}

template <typename T>
void p(T const &t, std::ostream &o= std::cout) { o<< t<< '\n'<< std::flush; }

} } // try using pwned::io::operator<<;

#endif

