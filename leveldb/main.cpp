#include <memory>
#include <iostream>
#include "leveldb.hpp"

int main()
{
  pwned::leveldb::DB db("data");
  db.put("1", "abc");
  db.put("2", "def");
  db.put("3", "fgrhi");
  std::unique_ptr<leveldb::Iterator> it(db.db-> NewIterator(db.read_options));
  leveldb::Slice start= "1";
  leveldb::Slice end= "3";
  for(it-> Seek(start); it-> Valid(); it-> Next())
  {
    leveldb::Slice key= it-> key();
    leveldb::Slice value= it-> value();
    if(db.options.comparator-> compare(key, end)> 0)
      break;
    else
    {
      std::cout<< std::string(key)<< "-> "<< std::string(value)<< '\n';
    }
  }
  std::cout<< it-> status().ok()<< std::endl;
}

