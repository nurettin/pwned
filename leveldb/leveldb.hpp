#ifndef PWNED_LEVELDB_HPP
#define PWNED_LEVELDB_HPP

#include <string>
#include <stdexcept>
#include <functional>
#include <memory>
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <leveldb/comparator.h>

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

  ~DB(){ delete db; }

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
  
  void each(::leveldb::Slice const &begin
    , ::leveldb::Slice const &end
    , std::function<void(std::string const &, std::string const &)> f)
  {
    std::unique_ptr< ::leveldb::Iterator> it(db-> NewIterator(read_options));
    for(it-> Seek(begin); it-> Valid(); it-> Next())
    {
      ::leveldb::Slice const &key= it-> key();
      if(options.comparator-> Compare(key, end)> 0)
        break;
      f(key.ToString(), it-> value().ToString());
    }
    check_status(it-> status());
  }

  void reverse_each(::leveldb::Slice const &rbegin
    , ::leveldb::Slice const &rend
    , std::function<void(std::string const &, std::string const &)> f)
  {
    std::unique_ptr< ::leveldb::Iterator> it(db-> NewIterator(read_options));
    it-> Seek(rbegin); 
    if(!it-> Valid())
      it-> SeekToLast(); 
    else
      it-> Prev();
    for(; it-> Valid(); it-> Prev())
    {
      ::leveldb::Slice const &key= it-> key();
      if(options.comparator-> Compare(rend, key)> 0)
        break;
      f(key.ToString(), it-> value().ToString());
    }
    check_status(it-> status());
  }

  void batch(std::function<void(::leveldb::WriteBatch &batch)> f)
  {
    ::leveldb::WriteBatch batch;
    f(batch);
    check_status(db-> Write(write_options, &batch));
  }

  void check_status(::leveldb::Status const &status)
  {
    if(!status.ok())
      throw std::runtime_error(status.ToString());
  }

};

} }

#endif

