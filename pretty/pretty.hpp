#ifndef PWNED_IO_HPP
#define PWNED_IO_HPP

#include <iostream>
#include <ostream>
#include <string>

namespace pwned { namespace pretty {

template <typename T>
void print(std::ostream &o, T const &t) { o<< t; }

void print(std::ostream &o, std::string const &s){ o<< '"'<< s<< '"'; }

template <typename K, typename V>
void print(std::ostream &o, std::pair<K, V> const &p)
{
  o<< '{'; print(o, p.first);
  o<< ": "; print(o, p.second);
  o<< '}';
}

template <typename I>
void print_loop(std::ostream &o, I b, I e)
{
  o<< '[';
  if(b== e)
  {
    o<< ']';
    return;
  }
  print(o, *b);
  ++ b;
  if(b== e)
  {
    o<< ']';
    return;
  }
  for(; b!= e; ++ b)
  {
    o<< ", ";
    print(o, *b);
  }
  o<< ']';
}

template <typename T>
void p(T const &t, std::ostream &o= std::cout) { o<< t<< '\n'<< std::flush; }

} } // pwned pretty

namespace std {

template <typename T, std::size_t N>
typename std::enable_if<std::is_array<T[]>::value && !std::is_same<T, char>::value && !std::is_same<T, wchar_t>::value, std::ostream &>::type
operator<< (std::ostream &o, T const (&a)[N])
{
  pwned::pretty::print_loop(o, &a[0], &a[N]);
  return o;
}

template <typename C, typename I= typename C::const_iterator>
typename std::enable_if<!std::is_same<C, std::string>::value, std::ostream &>::type
operator<< (std::ostream &o, C const &c)
{
  pwned::pretty::print_loop(o, c.begin(), c.end());
  return o;
}

} // std

#endif
