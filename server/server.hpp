#include <string>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <boost/optional.hpp>
#include <mongoose.h>

namespace pwned { namespace server {

struct Server
{
  mg_context* ctx;
  typedef std::function<std::string(mg_event*)> Event;
  typedef std::unordered_map<std::string, boost::optional<Event>> Routes;
  Routes routes;

  Server(char const* port)
  {
    char const* options[]{ "listening_ports", port, 0 };
    ctx = mg_start(options, &Server::event_handler, &routes);
  }

  ~Server(){ mg_stop(ctx); }

  void get(std::string const &uri, Event block)
  {
    routes.insert({ "GET_"+ uri, boost::optional<Event>(block) });
  }

  static std::string response(std::string const &content, std::string const &status= "200 OK")
  {
    std::ostringstream out;
    out<< "HTTP/1.1 "<< status<<"\r\n"
      << "Content-Type: text/plain\r\n"
      << "Content-Length: "<< content.size()<< "\r\n\r\n"
      << content;
    return out.str();
  }

  private:
  static int event_handler(mg_event* event)
  {
    if(event-> type!= MG_REQUEST_BEGIN)
      return 0;

    Routes* routes_ptr= static_cast<Routes*>(event-> user_data);
    auto block= (*routes_ptr)[std::string(event-> request_info-> request_method)+ "_"+ event-> request_info-> uri];
    if(!block) return 0;

    auto response= (*block)(event);
    mg_printf(event-> conn, "%s", response.c_str());
    return 1;
  }
};

} } // pwned server

