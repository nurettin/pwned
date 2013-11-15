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

