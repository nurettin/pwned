#ifndef PWNED_JSONCPP_HPP
#define PWNED_JSONCPP_HPP

#include <string>
#include <stdexcept>
#include <json/json.h>

namespace pwned { namespace jsoncpp {

struct Json
{
	::Json::Value root;
	::Json::Reader reader;
	::Json::FastWriter writer;

	Json(std::string const &);
	::Json::Value &document();
	std::string to_string();
	void operator= (std::string const &);
};

} }

#endif

