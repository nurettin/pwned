#include <gflags/gflags.h>
#include <server/server.hpp>

DEFINE_string(ports, "8080,4343s", "comma separated list of ports. Ex: 8080,4343s where s means SSL");
DEFINE_string(ssl_cert, "ssl_cert.pem", "SSL certificate if present.");

int main(int argc, char* argv[])
{
  google::SetUsageMessage(std::string(argv[0])+ "[ports] [certificate]");
  google::ParseCommandLineFlags(&argc, &argv, true);

  using namespace pwned::server;
  Server serve(FLAGS_ports.c_str());

  serve.Folder("/public");

  serve.Get("/", [](mg_event*, Params const &) {
    return Server::file("public/index.html", "text/html");
  });

  serve.Get("/data", [](mg_event*, Params const &) {
    return Server::response("{\"name\": \"onur\"}", "application/json");
  });

  std::cin.get();
}
