#ifndef ECTO1_HPP
#define ECTO1_HPP

#include <pwned/curl/curl.hpp>
#include <pwned/jsoncpp/jsoncpp.hpp>
#include <iostream>

namespace pwned { namespace ecto1 {

struct Ecto1
{
  std::string phantom_url;
  std::string session_id;
  std::string session_url;

  Ecto1(std::string const &phantom_url)
  : phantom_url(phantom_url)
  {
    std::string ret;
    pwned::jsoncpp::Json req;
    //There is a bug when using MSIE user agent https://github.com/ariya/phantomjs/issues/11827
    req["desiredCapabilities"]["phantomjs.page.settings.userAgent"]= "Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:25.0) Gecko/20100101 Firefox/25.0";
    pwned::curl::post(phantom_url+ "/session", ret, req.string().c_str());
    //TODO: submit bug report because post result doesn't have any session info
    //TODO: following code is a workaround, remove it when issue is fixed
    ret= pwned::curl::open(phantom_url+ "/sessions");
    pwned::jsoncpp::Json res(ret);
    if(res["value"].size()== 0)
      throw std::runtime_error("No sessions opened");
    session_id= res["value"][res["value"].size()- 1]["id"].asString();
    session_url= phantom_url+ "/session/"+ session_id;
  }

  void get(std::string const &url)
  {
    std::string ret;
    pwned::jsoncpp::Json req;
    req["url"]= url;
    pwned::curl::post(session_url+ "/url", ret, req.string().c_str());
    if(req["status"].asInt()!= 0)
      throw std::runtime_error("Error navigating to url: "+ ret);
  }

  std::string element(std::string const &use, std::string const &value)
  {
    std::string ret;
    pwned::jsoncpp::Json req;
    req["using"]= use;
    req["value"]= value;
    pwned::curl::post(session_url+ "/element", ret, req.string().c_str());
    pwned::jsoncpp::Json res(ret);
    if(res["status"].asInt()!= 0)
      throw std::runtime_error("Error getting element id: "+ ret);
    return res["value"]["ELEMENT"].asString();
  }
  
  void set_value(std::string const &element, std::string const &text)
  {
    std::string ret;
    pwned::jsoncpp::Json req;
    int i= 0;
    for(auto c: text)
      req["value"][i++]= std::string(1, c);
    pwned::curl::post(session_url+ "/element/"+ element+ "/value", ret, req.string().c_str());
    pwned::jsoncpp::Json res(ret);
    if(res["status"].asInt()!= 0)
      throw std::runtime_error("Error setting value: "+ ret);
  }

  void set_value_by_name(std::string const &name, std::string const &text)
  {
    set_value(element("name", name), text);
  }
  
  void click(std::string const &element)
  {
    std::string ret;
    pwned::curl::post(session_url+ "/element/"+ element+ "/click", ret, "");
    pwned::jsoncpp::Json res(ret);
    if(res["status"].asInt()!= 0)
      throw std::runtime_error("Error clicking element: "+ ret);
  }

  void click_by_name(std::string const &name)
  {
    click(element("name", name));
  }

  void click_by_id(std::string const &id)
  {
    click(element("id", id));
  }

  void top_frame()
  {
    std::string ret;
    pwned::jsoncpp::Json req;
    req["id"]= "null";
    pwned::curl::post(session_url+ "/frame", ret, req.string().c_str());
    pwned::jsoncpp::Json res(ret);
    if(res["status"].asInt()!= 0)
      throw std::runtime_error("Error switching to iframe: "+ ret);
  }

  void frame(int frame_number)
  {
    std::string ret;
    pwned::jsoncpp::Json req;
    req["id"]= frame_number;
    pwned::curl::post(session_url+ "/frame", ret, req.string().c_str());
    pwned::jsoncpp::Json res(ret);
    if(res["status"].asInt()!= 0)
      throw std::runtime_error("Error switching to iframe: "+ ret);
  }

  std::string screenshot()
  {
    pwned::jsoncpp::Json ret(pwned::curl::open(session_url+ "/screenshot"));
    if(ret["status"].asInt()!= 0)
      throw std::runtime_error("Error taking screenshot: "+ ret.string());
    return ret["value"].asString();
  }

  std::string screenshot_mime()
  {
    return "data:image/png;base64,"+ screenshot();
  }

  ~Ecto1()
  {
    std::string ret;
    pwned::curl::delete_(session_url, ret);
    pwned::jsoncpp::Json res(ret);
    if(res["status"].asInt()!= 0)
      throw std::runtime_error("Error destroying session: "+ ret);
  }
};

} }

#endif

