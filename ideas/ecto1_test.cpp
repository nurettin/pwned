#include <pwned/curl/curl.hpp>
#include <pwned/jsoncpp/jsoncpp.hpp>
#include <iostream>
#include <fstream>

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
    req["desiredCapabilities"]["phantomjs.page.settings.userAgent"]= "Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:25.0) Gecko/20100101 Firefox/25.0";
    pwned::curl::post(phantom_url+ "/session", ret, req.string().c_str());

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
    std::cout<< ret<< '\n';
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

  void screenshot()
  {
    pwned::jsoncpp::Json ret(pwned::curl::open(session_url+ "/screenshot"));
    std::ofstream file("shot.html");
    file<< "<img src=\"data:image/png;base64,"+ ret["value"].asString()+ "\" />";
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

int main()
{
  Ecto1 driver("http://localhost:4242");
  std::cout<< "session id: "<< driver.session_id<< '\n';
  driver.get("https://www.isbank.com.tr/Internet/");
  std::cout<< "typing username\n";
  driver.set_value_by_name("_ctl0:MusNoText", std::getenv("IS_USERNAME"));
  std::cout<< "typing password\n";
  driver.set_value_by_name("ParolaText", std::getenv("IS_PASSWORD"));
  std::cout<< "clicking submit\n";
  driver.click_by_name("_ctl0:SubeLogin01_btnGiris");
  // CepIAnahtarSifreText
  // _ctl0:Z6_IGA:TxtSoftOtp
  // _ctl0:Z6_IGA:SubeLogin02_CepAnahtarBtnGiris
  std::cout<< "how I shot web\n";
  driver.screenshot();
}

