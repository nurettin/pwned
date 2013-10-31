#ifndef PWNED_LEVELDB_HPP
#define PWNED_LEVELDB_HPP

#include <string>
#include <stdexcept>
#include <functional>
#include <memory>
#include <boost/optional.hpp>
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <leveldb/comparator.h>

namespace pwned { namespace leveldb {

//! Leveldb database operations
struct DB
{
  //! Directory of leveldb database
  std::string directory;
  //! Persistent write options
  ::leveldb::WriteOptions write_options;
  //! Persistent read options
  ::leveldb::ReadOptions read_options;
  //! Persistent options (create_if_missing=true by default)
  ::leveldb::Options options;
  //! Database instance
  ::leveldb::DB* db;

  //! Open an existing database.
  /*! Creates a blank database if database is missing.
  */
  DB(std::string const &directory)
  : directory(directory)
  {
    options.create_if_missing= true;
    check_status(::leveldb::DB::Open(options, directory, &db));
  }

  ~DB(){ delete db; }

  //! Record a key-value pair to the leveldb database.
  void put(::leveldb::Slice const &k, ::leveldb::Slice const &v)
  {
    check_status(db-> Put(write_options, k, v));
  }

  //! Get a value according to key if it exists.
  /*! Returns boost::none if there was an error getting the key.
  */
  boost::optional<std::string> get(::leveldb::Slice const &k)
  {
    std::string v;
    auto status= db-> Get(read_options, k, &v);
    if(status.ok())
      return v;
    return boost::none;
  }

  //! Remove a key/value pair from leveldb
  void remove(::leveldb::Slice const &k)
  {
    check_status(db-> Delete(write_options, k));
  }

  //! Call block on leveldb range of keys
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

  //! Call block on leveldb range of keys in reverse order
  void reverse_each(::leveldb::Slice const &rbegin
    , ::leveldb::Slice const &rend
    , std::function<void(std::string const &, std::string const &)> block)
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
      block(key.ToString(), it-> value().ToString());
    }
    check_status(it-> status());
  }

  //! Call given block as batch operation
  void batch(std::function<void(::leveldb::WriteBatch &batch)> block)
  {
    ::leveldb::WriteBatch batch;
    block(batch);
    check_status(db-> Write(write_options, &batch));
  }

  //! Check if leveldb is newly created.
  /*! Seeks to first element in the database and checks if the iterator is valid.
  */
  bool empty() const
  {
    std::unique_ptr< ::leveldb::Iterator> it(db->NewIterator(read_options));
    it-> SeekToFirst();
    if(it-> Valid())
      return false;
    return true;
  }

  //! Check if leveldb status is ok, if not, throw an exception.
  void check_status(::leveldb::Status const &status) const
  {
    if(!status.ok())
      throw std::runtime_error(status.ToString());
  }
};

} }

#endif

