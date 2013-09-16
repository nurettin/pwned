#ifndef PWNED_FORMICATOR_HPP
#define PWNED_FORMICATOR_HPP

#include <map>
#include <string>
#include <htmlcxx/html/ParserDom.h>
#include <hcxselect.h>
#include "curl.hpp"

namespace pwned{ namespace curl {

struct Formicator
{
  CURL* curl;
  htmlcxx::HTML::ParserDom parser;
  std::string domain;
  std::string user_agent;
  std::string page;
  tree<htmlcxx::HTML::Node> dom;
  hcxselect::Selector selector;

  Formicator(std::string const &domain, std::string const &user_agent= "Mozilla/4.0")
  : curl(curl_easy_init())
  , domain(domain)
  , user_agent(user_agent)
  {
    check(curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str()));
    check(curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1));
    check(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1));
    check(curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""));
  }
  
  void reset_session()
  {
    curl_easy_cleanup(curl);
    curl= curl_easy_init();
  }

  ~Formicator()
  {
    curl_easy_cleanup(curl);
  }

  void perform(std::function<void()> block)
  {
    page= "";
    block();
    dom= parser.parseTree(page);
    selector= hcxselect::Selector(dom);
  }

  void get(std::string const &url)
  {
    perform([&](){
      pwned::curl::get(domain+ "/"+ url, page, curl, true);
    });
  }

  void post(std::string const &url, Params const &params)
  {
    std::string params_string= params_to_string(params);
    perform([&](){
      pwned::curl::post(domain+ "/"+ url, page, params_string.c_str(), curl, true);
    });
  }

  hcxselect::Selection select(std::string const &css)
  {
    return selector.select(css);
  }

  tree_node_<htmlcxx::HTML::Node>* find(std::string const &css)
  {
    hcxselect::Selection selection= select(css);
    if(selection.empty()) return 0;
    return *select(css).begin();
  }

};

} } // pwned curl

#endif

