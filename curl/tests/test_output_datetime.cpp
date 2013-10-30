#include <iostream>
#include <boost/locale.hpp>
#include <boost/date_time.hpp>

using namespace boost;

int main(int argc, char **argv) {
   locale::generator gen;
   std::locale::global(gen(""));

   locale::date_time now;
   std::cout.imbue(std::locale());       
   std::cout << locale::as::ftime("%H %c %Z")<< now << std::endl;

  const boost::posix_time::ptime now2=
      boost::posix_time::second_clock::local_time();
  std::cout<< now2<< std::endl;;

   return 0;
}

