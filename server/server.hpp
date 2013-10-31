#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <boost/optional.hpp>
#include <re2/filtered_re2.h>
#include <mongoose.h>

namespace pwned { namespace server {

struct Router
{
  typedef std::map<std::string, std::string> Params;
  typedef std::function<std::string(mg_event*, Params const &)> Event;
  std::unique_ptr<re2::FilteredRE2> filter;
  std::vector<int> regex_indexes;
  std::vector<std::shared_ptr<RE2>> regexes;
  std::vector<Event> events;
  re2::RE2 re_param_to_regex;
  re2::StringPiece sp_param_to_regex;
  re2::RE2::Options options;

  Router()
  : filter(new re2::FilteredRE2)
  , re_param_to_regex("(:([^/]+))")
  , sp_param_to_regex("(?P<\\2>[^/]+)")
  {}

  void add(std::string route, Event block)
  {
    re2::RE2::GlobalReplace(&route, re_param_to_regex, sp_param_to_regex);
    filter.reset(new re2::FilteredRE2);
    if(regex_indexes.empty())
      regex_indexes.push_back(0);
    else
      regex_indexes.push_back(regex_indexes.back()+ 1);
    regexes.push_back(std::shared_ptr<re2::RE2>(new re2::RE2(route)));
    events.push_back(block);

    for(std::size_t i= 0; i< regex_indexes.size(); ++ i)
      check(filter-> Add(regexes[i]-> pattern(), options, &regex_indexes[i]));
    std::vector<std::string> strs;
    filter-> Compile(&strs);
  }

  boost::optional<Params> extract_uri_params(std::string const &uri, re2::RE2 const &re)
  {
    re2::StringPiece input(uri);
    int group_size = re.NumberOfCapturingGroups();
    std::vector<re2::StringPiece> ws(group_size);
    std::vector<re2::RE2::Arg> argv(group_size);
    std::vector<re2::RE2::Arg*> args(group_size);

    for(int i= 0; i< group_size; ++ i) 
    {
      args[i]= &argv[i];
      argv[i]= &ws[i];
    }
    bool ok= re2::RE2::FullMatchN(input, re, &args[0], group_size);
    if(!ok)
      return boost::none;
    std::map<std::string, std::string> result;
    for(auto &cgn: re.CapturingGroupNames())
      result[cgn.second]= ws[cgn.first- 1].ToString();
    return result;
  }
  
  boost::optional<std::pair<Event, Params>> match(std::string const &uri)
  {
    std::vector<int> matches;
    bool ok= filter-> AllMatches(uri, regex_indexes, &matches);
    if(!ok) return boost::none;
    for(int match: matches)
    {
      auto uri_params= extract_uri_params(uri, *regexes[match]);
      if(!uri_params) continue;
      return std::make_pair(events[match], *uri_params);
    }
    return boost::none;
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
  Router router;

  Server(char const* port)
  {
    char const* options[]{ "listening_ports", port, 0 };
    ctx = mg_start(options, &Server::event_handler, &router);
  }

  ~Server(){ mg_stop(ctx); }

  void Get(std::string const &uri, pwned::server::Router::Event block){ router.add("GET_"+ uri, block); }
  void Post(std::string const &uri, pwned::server::Router::Event block){ router.add("POST_"+ uri, block); }
  void Put(std::string const &uri, pwned::server::Router::Event block){ router.add("PUT_"+ uri, block); }
  void Patch(std::string const &uri, pwned::server::Router::Event block){ router.add("PATCH_"+ uri, block); }
  void Delete(std::string const &uri, pwned::server::Router::Event block){ router.add("DELETE_"+ uri, block); }

  static std::string response(std::string const &content
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

    Router* router_ptr= static_cast<Router*>(event-> user_data);

    // use router to parse uri and uri parameters
    auto pair_block_param= router_ptr-> match(std::string(event-> request_info-> request_method)+ "_"+ event-> request_info-> uri);
    if(!pair_block_param) return 0;
    
    // read post data
    char post_data[16* 1024] {}; // 16 kb max post data size
    int post_data_len= mg_read(event-> conn, post_data, sizeof(post_data));

    std::string str_post_data(post_data, post_data+ post_data_len);
    std::istringstream parser(str_post_data);
    std::string key, value;
    bool ok= false;
    while(1)
    {
      ok= std::getline(parser, key, '='); if(!ok) break;
      ok= std::getline(parser, value, '&'); 
      pair_block_param-> second.insert(std::make_pair(key, value));
      if(!ok) break;
    }
    try
    {
    auto response= pair_block_param-> first(event, pair_block_param-> second);
    mg_printf(event-> conn, "%s", response.c_str());
    } catch(std::exception &ex)
    {
      std::cerr<< "Error: "<< ex.what()<< '\n';
      return 0;
    }
    return 1;
  }
};

} } // pwned server

