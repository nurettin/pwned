#include <server/server.hpp>

int main()
{
  using pwned::server::Server;
  Server server("8080");
  server.get("/hello", [](mg_event*){
    return Server::response("merhaba sevgilim!");
  });
  std::cin.get();
}

