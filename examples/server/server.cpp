#include <server/server.hpp>

int main()
{
  using pwned::server::Server;
  Server server("8080");
  server.get("/users", [](mg_event*, pwned::server::Router::Params const &params){
    return Server::response("<html><body>This is a list of users:<table><tr><td>pwned</td></tr><tr><td>edoceo`</td></tr></table></body></html>", "text/html");
  });
  server.get("/user/:id", [](mg_event*, pwned::server::Router::Params const &params){
    return Server::response("This is user number: "+ params.at("id"));
  });
  server.get("/user/:id/post/:post_id", [](mg_event*, pwned::server::Router::Params const &params){
    return Server::response(
      "This is user number: "+ params.at("id")+ 
      " and post number: "+ params.at("post_id"));
  });
  std::cin.get();
}

