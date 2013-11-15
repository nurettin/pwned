#include <pew/pew.hpp>
#include <server/server.hpp>

int main()
{
  std::string dog("hi this is dog ");
  for(int n= 22; n--; dog+= dog);
  pwned::pew::Pew pew;
  pew.bench("boost iostreams", [=]{
    pwned::server::Server::compress(dog);
  });
  pew.bench("raw zlib stream", [=]{
    pwned::server::Server::compress2(dog);
  });
  pew.run(15);
}

/* outputs:

Tests repeated: 15 times, averages:
1. boost iostreams  674ms 93.07%
2. raw zlib stream  725ms 100.00%

*/

