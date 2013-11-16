#include <string>
#include <vector>
#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <zlib.h>

namespace pwned { namespace server {

struct Server
{
  static std::string compress(std::string const &data)
  {
    using namespace boost::iostreams;
    filtering_streambuf<input> in;
    in.push(gzip_compressor());
    in.push(boost::make_iterator_range(data));
    std::ostringstream out;
    copy(in, out);
    return out.str();
  }

  static std::string compress2(std::string const &data)
  {
    int outlen= ::compressBound(data.size())+ 18;
    std::vector<char> out(outlen);
    z_stream s {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    s.next_in = (Bytef*)data.c_str();
    s.avail_in = data.size();
    int res = deflateInit2(&s, Z_DEFAULT_COMPRESSION, Z_DEFLATED, (15+ 16), 8, Z_DEFAULT_STRATEGY);
    if (res!= Z_OK) return "";
    do {
      s.next_out = (Bytef*)(&out[0]) + s.total_out;
      s.avail_out = outlen - s.total_out;
      res = deflate(&s, Z_FINISH);
    } while ( res == Z_OK );
    deflateEnd(&s);
    return std::string(out.begin(), out.begin()+ s.total_out);
  }
};

} } // pwned server

