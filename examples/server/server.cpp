#include <server/server.hpp>

int main()
{
  using namespace pwned::server;
  Server server("8080");

  server.get("/", [](mg_event*, Router::Params const &params) {
    return Server::response("<a href='/add/42/42'>Add 42 + 42</a>", "text/html");
  });

  server.get("/add/:first/:second", [](mg_event*, Router::Params const &params){
    int first= std::stoi(params.at("first"));
    int second= std::stoi(params.at("second"));
    std::ostringstream out;
    out<< first<< " + "<< second<< " = "<< (first+ second)<< '\n';
    return Server::response(out.str());
  });

  std::cin.get();
}

