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

  DB(std::string const &directory)
  : directory(directory)
  {
    options.create_if_missing= true;
    check_status(::leveldb::DB::Open(options, directory, &db));
  }

  void put(::leveldb::Slice const &k, ::leveldb::Slice const &v)
  {
    check_status(db-> Put(write_options, k, v));
  }

  std::string get(::leveldb::Slice const &k)
  {
    std::string v;
    check_status(db-> Get(read_options, k, &v));
    return v;
  }

  void remove(::leveldb::Slice const &k)
  {
    check_status(db-> Delete(write_options, k));
  }
  
  // TODO: Add range iteration
  void check_status(::leveldb::Status const &status)
  {
    if(!status.ok())
      throw std::runtime_error(status.ToString());
  }
};

} }

#endif

