#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <zlib.h>

std::string hex(std::string const &data)
{
  std::ostringstream out;
  auto begin= data.cbegin();
  auto end= data.cend();
  out<< std::hex<< std::setfill('0')<< std::setw(2)<< static_cast<unsigned>(static_cast<unsigned char>(*begin));
  ++ begin;
  if(begin== end) return out.str();
  for(; begin!= end; ++ begin)
    out<< ' '<< std::setw(2)<< static_cast<unsigned>(static_cast<unsigned char>(*begin));
  return out.str();
}

std::string compress(std::string const &data)
{
  std::vector<unsigned char> b(data.size());
  // deflate
  // zlib struct
  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  strm.avail_in = data.size()+ 1; // size of input, string + terminator
  strm.next_in = (Bytef *)data.data(); // input char array
  strm.avail_out = b.size(); // size of output
  strm.next_out = &b[0]; // output char array
  int status= 0;
  bool compressed= false;
  int windowBits = 15    // the default
                 + 16;   // for gzip header; 
                         // see deflateInit2() documentation in zlib.h
  if( ( status = deflateInit2( &strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, windowBits, 8, Z_DEFAULT_STRATEGY ) ) == Z_OK ) {
      // run until the buffer is processed or there is an error
      while( ( status = deflate( &strm, Z_FINISH ) ) == Z_OK );
      if( status == Z_STREAM_END )
          compressed = true;
  }
  deflateEnd(&strm);
  std::cout<< strm.total_out<< '\n';
  return std::string(b.begin(), b.begin()+ strm.total_out);
}

int main()
{
  std::string compressed= compress("hi this is a test");
  std::cout<< "size: "<< compressed.size()<< '\n';
  std::cout<< hex(compressed)<< '\n';
}

