#include <string>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <memory>
#include <boost/optional.hpp>
#include <re2/filtered_re2.h>
#include <mongoose.h>

namespace pwned { namespace server {

struct Router
{
  typedef std::function<std::string(mg_event*)> Event;
  std::unique_ptr<re2::FilteredRE2> filter;
  std::vector<int> regex_indexes;
  std::vector<std::string> regexes;
  std::vector<Event> events;

  Router()
  : filter(new re2::FilteredRE2)
  {}

  void add(std::string const &regex)
  {
    filter.reset(new re2::FilteredRE2);
    if(regex_indexes.empty())
      regex_indexes.push_back(0);
    else
      regex_indexes.push_back(regex_indexes.back()+ 1);
    regexes.push_back(regex);

    for(std::size_t i= 0; i< regex_indexes.size(); ++ i)
      check(filter-> Add(regexes[i], RE2::Options(), &regex_indexes[i]));
    std::vector<std::string> strs;
    filter-> Compile(&strs);
  }

  int match(std::string const &url)
  {
    return filter-> FirstMatch(url, regex_indexes);
  }

  private:
  void check(RE2::ErrorCode const &code)
  {
    if(code== RE2::NoError)
      return;
    throw std::runtime_error("Error adding regex: "+ std::string(1, '0'+ code));
  }
};

struct Server
{
  mg_context* ctx;
  typedef std::function<std::string(mg_event*)> Event;
  typedef std::unordered_map<std::string, boost::optional<Event>> Routes;
  Routes routes;
  Router router;

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

  static std::string response(
    std::string const &content
    , std::string const &content_type= "text/plain"
    , std::string const &status= "200 OK")
  {
    std::ostringstream out;
    out<< "HTTP/1.1 "<< status<< "\r\n"
      << "Content-Type: "<< content_type<< "\r\n"
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

