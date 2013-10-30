#include <iostream>
#include <sstream>
#include <string>
#include <boost/locale.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

using namespace boost;

boost::posix_time::time_duration get_utc_offset() 
{
  using namespace boost::posix_time;
  typedef boost::date_time::c_local_adjustor<ptime> local_adj;
  const ptime utc_now = second_clock::universal_time();
  const ptime now = local_adj::utc_to_local(utc_now);
  return now - utc_now;
}

std::string get_utc_offset_string()
{
  std::stringstream out;
  using namespace boost::posix_time;
  time_facet* tf = new time_facet();
  tf->time_duration_format("%+%H:%M");
  out.imbue(std::locale(out.getloc(), tf));
  out << get_utc_offset();
  return out.str();
}

int main(int argc, char **argv) {
   locale::generator gen;
   std::locale::global(gen(""));
   locale::date_time now;
   std::cout.imbue(std::locale());
   std::cout<< locale::as::ftime("%e %b %Y %T ")
    << now<< get_utc_offset_string()
    << std::endl;
}

