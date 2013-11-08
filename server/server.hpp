#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <re2/filtered_re2.h>
#include <mongoose.h>

namespace pwned { namespace server {

enum class FolderType
{
  ROOT, NESTED
};

typedef std::map<std::string, std::string> Params;

struct Router
{
  typedef std::function<std::string(mg_event*, Params const &)> Event;
  std::unique_ptr<re2::FilteredRE2> filter;
  std::vector<int> regex_indexes;
  std::vector<std::shared_ptr<RE2>> regexes;
  std::vector<Event> events;
  std::unordered_map<std::string, boost::optional<Event>> fast_events;
  re2::RE2 re_has_param;
  re2::RE2 re_param_to_regex;
  re2::StringPiece sp_param_to_regex;
  re2::RE2::Options options;

  Router()
  : filter(new re2::FilteredRE2)
  , re_has_param("(:[^/]+)")
  , re_param_to_regex("(:([^/]+))")
  , sp_param_to_regex("(?P<\\2>[^/]+)")
  {}

  bool fast(std::string const &route)
  {
    return !re2::RE2::PartialMatch(route, re_has_param, (void*)0);
  }

  void add(std::string route, Event block)
  {
    // remove trailing slash when adding
    if(route.size()> 5 && route.back()== '/') route.resize(route.size()- 1);
    // std::printf("adding route: %s\n", route.c_str()); std::fflush(0);
    if(fast(route))
    {
      fast_events.insert(std::make_pair(route, block));
      return;
    }

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

  boost::optional<Params> rest_params(std::string const &uri, re2::RE2 const &re)
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

  boost::optional<std::pair<Event, Params>> match(std::string uri)
  {
    // remove trailing slash when matching
    if(uri.size()> 5 && uri.back()== '/') uri.resize(uri.size()- 1);
    // std::printf("matching route: %s\n", uri.c_str()); std::fflush(0);
    auto event= fast_events[uri];
    if(event)
    {
      // std::printf("fast match found!\n"); std::fflush(0);
      return std::make_pair(*event, Params());
    }
    if(regex_indexes.empty())
    {
      // std::printf("there are no REST routes\n"); std::fflush(0);
      return boost::none;
    }
    std::vector<int> matches;
    bool ok= filter-> AllMatches(uri, regex_indexes, &matches);
    if(!ok) return boost::none;
    for(int match: matches)
    {
      auto uri_rest_params= rest_params(uri, *regexes[match]);
      if(!uri_rest_params) continue;
      // std::printf("parameter match found!\n"); std::fflush(0);
      return std::make_pair(events[match], *uri_rest_params);
    }
    // std::printf("no match found!\n"); std::fflush(0);
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

  Server(char const* listening_ports= "8080,4343s", char const* ssl_certificate= "ssl_cert.pem")
  {
    std::ios_base::sync_with_stdio(false);
    std::ifstream cert(ssl_certificate);
    if(cert.is_open())
    {
      cert.close();
      std::cout<< "Found certificate: "<< ssl_certificate<< '\n';
      char const* options[]{ "listening_ports", listening_ports, "ssl_certificate", ssl_certificate, 0 };
      ctx = mg_start(options, &Server::event_handler, &router);
    }
    else
    {
      char const* options[]{ "listening_ports", listening_ports, 0 };
      ctx = mg_start(options, &Server::event_handler, &router);
    }
    if(!ctx)
      throw std::runtime_error("Error starting mongoose!");
    std::cout<< "Started pwned backed by mongoose "<< mg_version()<< " on port(s) "<< listening_ports<< std::endl;
  }

  ~Server(){ mg_stop(ctx); }

  void Get(std::string const &uri, pwned::server::Router::Event block){ router.add("GET_"+ uri, block); }
  void Post(std::string const &uri, pwned::server::Router::Event block){ router.add("POST_"+ uri, block); }
  void Put(std::string const &uri, pwned::server::Router::Event block){ router.add("PUT_"+ uri, block); }
  void Patch(std::string const &uri, pwned::server::Router::Event block){ router.add("PATCH_"+ uri, block); }
  void Delete(std::string const &uri, pwned::server::Router::Event block){ router.add("DELETE_"+ uri, block); }

  void Folder(std::string const &folder, FolderType folder_type= FolderType::NESTED)
  { 
    using namespace boost::filesystem;
    using namespace boost::algorithm;
    auto current_path= boost::filesystem::current_path();
    auto current_path_str_size= current_path.string().size();
    if(folder_type== FolderType::ROOT) current_path_str_size+= folder.size();
    recursive_directory_iterator begin(current_path/ folder), end;

    for(; begin!= end; ++ begin)
    {
      auto path= begin-> path();
      if(is_directory(begin-> status())) continue;
      auto path_str= path.string().substr(current_path_str_size);
      auto mime_str= mime(path_str);
      auto file_path= path_str;
      if(folder_type== FolderType::ROOT) file_path= folder+ path_str;
      auto file_str= trim_left_copy_if(file_path, is_any_of("/"));
      // std::printf("Adding folder: %s\n", file_str.c_str()); std::fflush(0);
      router.add("GET_"+ path_str, [file_str, mime_str](mg_event*, Params const &) {
        return file(file_str, mime_str);
      });
    }
  }

  static std::string response(std::string const &content
    , std::string const &content_type= "text/plain"
    , std::string const &status= "200 OK")
  {
    std::ostringstream out;
    out<< "HTTP/1.1 "<< status<< "\r\n"
      << "Content-Type: "<< content_type<< "\r\n"
      << "Content-Length: "<< content.size()<< "\r\n"
      << "Connection: keep-alive\r\n"
      << "Server: pwned/mongoose"<< mg_version()<< "\r\n\r\n"
      << content;
    return out.str();
  }

  static std::string redirect(std::string const &uri
    , std::string const &status= "302 Found")
  {
    std::ostringstream out;
    out<< "HTTP/1.1 "<< status<< "\r\n"
      << "Location: "<< uri<< "\r\n\r\n";
    return out.str();
  }

  static std::string mime(std::string const &file_name)
  {
    using namespace boost::algorithm;
    if(ends_with(file_name, ".html")|| ends_with(file_name, ".xhtml")|| ends_with(file_name, ".htm"))
      return "text/html";
    else if(ends_with(file_name, ".css"))
      return "text/css";
    else if(ends_with(file_name, ".js"))
      return "text/javascript";
    return "text/plain";
  }

  static std::string file(std::string const &file_name, std::string const &content_type)
  {
    std::ifstream file(file_name, std::ios::binary| std::ios::ate);
    if(!file.is_open()) 
    {
      // std::printf("file not found: %s\n", file_name.c_str()); std::fflush(0);
      return response("", "", "404 Not Found");
    }
    auto size= file.tellg();
    file.seekg(0, std::ios::beg);
    std::string buffer(size, 0);
    file.read(&buffer[0], size);
    return response(buffer, content_type);
  }

  static std::string file(std::string const &file_name)
  {
    return file(file_name, mime(file_name));
  }

  static std::string now()
  {
    std::time_t now= std::time(0);
    std::tm* now_tm= std::localtime(&now);
    char buf[42];
    std::strftime(buf, 42, "%Y-%m-%d %X", now_tm);
    return buf;
  }

  private:
  static int event_handler(mg_event* event)
  {
    if(event-> type!= MG_REQUEST_BEGIN)
      return 0;

    Router* router_ptr= static_cast<Router*>(event-> user_data);

    std::string uri(event-> request_info-> uri);
    std::string request_method(event-> request_info-> request_method);
    // std::printf("%s %s %s\n", now().c_str(), request_method.c_str(), uri.c_str());

    // use router to parse uri and uri parameters
    auto pair_block_param= router_ptr-> match(request_method+ "_"+ uri);
    if(!pair_block_param) return 0;

    std::string data;
    if(request_method== "GET" && event-> request_info-> query_string!= 0)
      // read get data
      data= event-> request_info-> query_string;
    else if(request_method== "POST"|| request_method== "PUT")
    {
      // read post data
      char post_data[16* 1024] {}; // 16 kb max post data size, because, why not?
      int post_data_len= mg_read(event-> conn, post_data, sizeof(post_data));
      data.assign(post_data, post_data_len);
    }

    // parse data if any
    if(!data.empty())
    {
      std::istringstream parser(data);
      std::string key, value;
      bool ok= false;
      while(1)
      {
        ok= std::getline(parser, key, '='); if(!ok) break;
        ok= std::getline(parser, value, '&');
        pair_block_param-> second.insert(std::make_pair(key, value));
        if(!ok) break;
      }
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

