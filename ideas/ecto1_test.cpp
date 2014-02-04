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
/*
  void screenshot()
  {
    pwned::jsoncpp::Json ret(pwned::curl::open(session_url+ "/screenshot"));
    std::ofstream file("shot.html");
    file<< "<img src=\"data:image/png;base64,"+ ret["value"].asString()+ "\" />";
  }
*/
  ~Ecto1()
  {
    std::string ret;
    pwned::curl::delete_(session_url, ret);
    pwned::jsoncpp::Json res(ret);
    if(res["status"].asInt()!= 0)
      throw std::runtime_error("Error destroying session: "+ ret);
  }
};

struct Is
{
  Ecto1 driver;

  Is()
  : driver("http://localhost:4242")
  {}

  void login(std::string const &password)
  {
    std::cout<< "session id: "<< driver.session_id<< '\n';
    driver.get("https://www.isbank.com.tr/Internet/");
    std::cout<< "typing username\n";
    driver.set_value_by_name("_ctl0:MusNoText", std::getenv("IS_USERNAME"));
    std::cout<< "typing password\n";
    driver.set_value_by_name("ParolaText", std::getenv("IS_PASSWORD"));
    std::cout<< "clicking submit\n";
    driver.click_by_name("_ctl0:SubeLogin01_btnGiris");
    std::cout<< "entering password2\n";
    driver.set_value_by_name("CepIAnahtarSifreText", std::getenv("IS_PASSWORD2"));
    std::cout<< "entering generated password\n";
    driver.set_value_by_name("_ctl0:Z6_IGA:TxtSoftOtp", password);
    std::cout<< "clicking submit\n";
    driver.click_by_name("_ctl0:Z6_IGA:SubeLogin02_CepAnahtarBtnGiris");
  }

  void try_login(std::string const &password)
  {
    try {
      login(password);
    } catch(std::exception &ex) {
      std::cerr<< "Error logging in: "<< ex.what()<< '\n';
      std::cerr<< "Trying again...\n";
      login(password);
    }
  }

  void home()
  {
    std::cout<< "top frame\n";
    driver.top_frame();
    std::cout<< "clicking home page\n";
    driver.click_by_id("Internet_lblGiris");
  }
  
  void buy_stock(std::string const &name)
  {
    try {
    home();
    std::cout<< "clicking yatirim\n";
    driver.click_by_id("LinkId_menu_MM_YATIRIM_03_");
    std::cout<< "clicking hisse senedi\n";
    driver.click_by_id("LinkId_menu_SM_HISSE_SENEDI_03_01_");
    std::cout<< "clicking alis\n";
    driver.click_by_id("LinkId_menu_GA00_03_01_01_");
    std::cout<< "frame 1\n";
    driver.frame(1);
    std::cout<< "entering stock name\n";
    driver.set_value_by_name("_ctl0:SmartSearchControl:SmartSearchControl_SmartSearchTextbox", name);
    } catch(std::exception &ex) {
    std::cerr<< "error clicking something: "<< ex.what()<< '\n';
    }
    driver.screenshot();
  }

  void quit()
  {
    driver.top_frame();
    driver.click_by_id("Internet_lblSLogout");
  }
};

int main()
{
  Is is;
  std::cout<< "Generated password: ";
  std::string generated_password;
  std::getline(std::cin, generated_password);
  is.try_login(generated_password);
  is.buy_stock("ADESE");
  is.quit();
}

