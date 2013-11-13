#include <iostream>
#include <vector>
#include <string>
#include <zlib.h>

int main()
{
  std::string data= "hi this is a test";
  std::vector<unsigned char> in(data.begin(), data.end());
  std::vector<unsigned char> out(::compressBound(in.size()));
  auto out_size= out.size();
  int result= compress(&out[0], &out_size, &in[0], in.size());
  switch(result)
  {
    case Z_OK:
      std::cout<< "Success!\n";
      break;
    case Z_MEM_ERROR:
      std::cout<< "Not enough memory!\n";
      break;
    case Z_BUF_ERROR:
      std::cout<< "Buffer too small!\n";
      break;
  }
  std::cout<< data.size()<< " compressed to: "<< out_size<< '\n';
}

