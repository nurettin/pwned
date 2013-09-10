#include <stdexcept>
#include <string>
#include <leveldb/db.h>
#include "leveldb.hpp"

namespace pwned { namespace leveldb {

DB::DB(std::string const &directory)
: directory(directory)
{
	options.create_if_missing= true;
	check_status(::leveldb::DB::Open(options, directory, &db));
}

void DB::put(::leveldb::Slice const &k, ::leveldb::Slice const &v)
{
	check_status(db-> Put(write_options, k, v));
}

std::string DB::get(::leveldb::Slice const &k)
{
	std::string v;
	check_status(db-> Get(read_options, k, &v));
	return v;
}

void DB::remove(::leveldb::Slice const &k)
{
	check_status(db-> Delete(write_options, k));
}

void DB::check_status(::leveldb::Status const &status)
{
	if(!status.ok())
		throw std::runtime_error(status.ToString());
}

::leveldb::WriteOptions &DB::WriteOptions()
{
	return write_options;
}

::leveldb::Options &DB::Options()
{
	return options;
}

} } // pwned

