#include <iostream>
#include <fstream>
#include <pwned/curl/formicator.hpp>

int main()
{
  // finds turkish names for countries by crawling wikipedia
  pwned::curl::Formicator crawl("http://tr.wikipedia.org");
  crawl.get("wiki/%C3%9Clkeler_listesi");
  auto links= crawl.select("table.wikitable td:nth-child(2) > a");
  
  for(auto &a: links)
  {
    if(a== 0) continue;
    auto &data= a-> data;
    data.parseAttributes();
    std::cout<< data.attributes().at("title")<< ' ';
  }
}

