#ifndef ENTERPRICE_IO_MULTICANDLESTREAM_HPP
#define ENTERPRICE_IO_MULTICANDLESTREAM_HPP

#include <unordered_map>
#include "candle_stream.hpp"

namespace pwned{ namespace enterprice{

struct MultiCandleStream
{
  std::unordered_map<std::string, CandleStream> streams;
  typedef std::function<void(std::string const &, unsigned long, double, double, double, double, double)> block_t;
  block_t block;
  unsigned long period;
  
  MultiCandleStream(unsigned long period, block_t block)
  : block(block)
  , period(period)
  {}

  void data(std::string const &code, unsigned long ts, double price, double amount)
  {
    {
      auto stream= streams.find(code);
      if(stream!= streams.end())
      {
        stream-> second.data(ts, price, amount);
        return;
      }
    }
    CandleStream stream(period, [=](unsigned long ts, double o, double h, double l, double c, double v){ block(code, ts, o, h, l, c, v); });
    stream.init_data(ts, price, amount);
    streams.insert({ code, stream });
  }

  void call() const
  {
    for(auto &kv: streams)
      kv.second.call();
  }
};

} } // pwned::enterprice

#endif

