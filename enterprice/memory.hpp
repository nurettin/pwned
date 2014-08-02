#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <memory>

namespace pwned{ namespace memory{ 

template <typename T, typename F>
std::unique_ptr<T, F> gc(T* p, F f)
{
  return std::unique_ptr<T, F>(p, f);
}

} } // pwned::memory

#endif

