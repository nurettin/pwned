#include <ctime>
#include <ostream>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

boost::posix_time::ptime local_ptime_from_utc_time_t(std::time_t const t)
{
    using boost::date_time::c_local_adjustor;
    using boost::posix_time::from_time_t;
    using boost::posix_time::ptime;
    return c_local_adjustor<ptime>::utc_to_local(from_time_t(t));
}

int main()
{
    using boost::posix_time::to_simple_string;
    using boost::posix_time::from_time_t;

    std::time_t t;
    std::time(&t); // initalize t as appropriate
    std::cout
        << "utc:   "
        << to_simple_string(from_time_t(t))
        << "\nlocal: "
        << to_simple_string(local_ptime_from_utc_time_t(t))
        << std::endl;
}


