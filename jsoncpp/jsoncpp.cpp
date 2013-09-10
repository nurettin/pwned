#include "jsoncpp.hpp"

namespace pwned { namespace jsoncpp {

Json::Json(std::string const &json)
{
	this-> operator=(json);
}

::Json::Value &Json::document()
{
	return root;
}

std::string Json::to_string()
{
	return writer.write(root);	
}

void Json::operator= (std::string const &json)
{
	bool ok= reader.parse(json, root);
	if(!ok)
		throw std::runtime_error(reader.getFormattedErrorMessages());
}

} }

