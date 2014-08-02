#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <memory>

template <typename T, typename F>
std::unique_ptr<T, F> gc(T* p, F f)
{
  return std::unique_ptr<T, F>(p, f);
}

#ifdef MEMORY_HPP_TEST

#include <cassert>

int main()
{
  static int set= 0;
  { struct wtf{ ~wtf(){ set= 1; } } omg; }
  assert(set== 1);
}

#endif

#endif

