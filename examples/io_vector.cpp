#include <vector>
#include "../io/io.hpp"

int main()
{
  std::vector<int> wtf { 1, 2, 3 };
  pwned::io::p(wtf);
  wtf.push_back(42);
  pwned::io::p(wtf);
}

