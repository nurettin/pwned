#include <string>
#include <iostream>

int main()
{
  std::string uri= "http://localhost:8080/?param1=wtf&param2=omg";
  auto get_params_start= uri.find_first_of("?");
  std::cout<< "get_params_start: "<< get_params_start
    << "\nuri: "<< uri.substr(0, get_params_start)
    << "\nget_params: "<< uri.substr(get_params_start)<< '\n';
}

