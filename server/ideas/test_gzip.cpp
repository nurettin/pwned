#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

std::string compress(std::string const &data)
{
  using namespace boost::iostreams;
  filtering_streambuf<input> in;
  in.push(gzip_compressor());
  in.push(boost::make_iterator_range(data));
  std::ostringstream out;
  copy(in, out);
  return out.str();
}

// g++ test_gzip.cpp -l boost_iostreams && ./a.out | gunzip
int main()
{
  std::string content= "compress this piece of data please";
  for(int n= 5; n--; content+= content);
  std::string compressed= compress(content);
  std::cout<< compressed;
}
