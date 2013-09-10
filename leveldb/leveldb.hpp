#ifndef PWNED_LEVELDB_HPP
#define PWNED_LEVELDB_HPP

#include <string>
#include <stdexcept>
#include <leveldb/db.h>

namespace pwned { namespace leveldb {

struct DB
{
	std::string directory;
	::leveldb::WriteOptions write_options;
	::leveldb::ReadOptions read_options;
	::leveldb::Options options;
	::leveldb::DB* db;

	DB(std::string const &);
	void put(::leveldb::Slice const &, ::leveldb::Slice const &);
	std::string get(::leveldb::Slice const &);
	void remove(::leveldb::Slice const &);
	void check_status(::leveldb::Status const &);
	::leveldb::WriteOptions &WriteOptions();
	::leveldb::Options &Options();
};

} }

#endif

