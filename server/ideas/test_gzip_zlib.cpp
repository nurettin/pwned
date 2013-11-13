#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <zlib.h>

std::string hex(std::string const &data)
{
  static auto remove_sign= [](char byte){ return static_cast<unsigned>(static_cast<unsigned char>(byte)); };
  std::ostringstream out;
  auto begin= data.cbegin();
  auto end= data.cend();
  out<< std::hex<< std::setfill('0')<< std::setw(2)<< remove_sign(*begin);
  ++ begin;
  if(begin== end) return out.str();
  for(; begin!= end; ++ begin)
    out<< ' '<< std::setw(2)<< remove_sign(*begin);
  return out.str();
}

std::string compress(std::string const &data)
{
  int outlen= ::compressBound(data.size())+ 18;
  std::vector<char> out(outlen);
  z_stream s {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  s.next_in = (Bytef*)data.c_str(); // Pointer to input bytes
  s.avail_in = data.size(); // Number
  int res = deflateInit2(&s, Z_DEFAULT_COMPRESSION, Z_DEFLATED, (15+16), 8, Z_DEFAULT_STRATEGY);
  if (res!= Z_OK) return "";
  do {
    s.next_out = (Bytef*)(&out[0]) + s.total_out;
    s.avail_out = outlen - s.total_out;
    res = deflate(&s, Z_FINISH);
  } while ( res == Z_OK );
  deflateEnd(&s);
  return std::string(out.begin(), out.begin()+ s.total_out);
}

int main()
{
  std::string compressed= compress("hi this is a test");
  std::cout<< compressed.size()<< '\n';
  std::cout<< hex(compressed)<< '\n';
}

