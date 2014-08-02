#ifndef ENTERPRICE_IO_MULTICANDLESTREAM_HPP
#define ENTERPRICE_IO_MULTICANDLESTREAM_HPP

#include <unordered_map>
#include "candle_stream.hpp"

namespace enterprice{

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

} // enterprice

#ifdef ENTERPRICE_IO_MULTICANDLESTREAM_HPP_TEST

#include <vector>
#include <tuple>
#include <cassert>
#include <iostream>

int main()
{
  typedef std::vector<std::tuple<std::string, unsigned long, double, double, double, double, double>> data_t;
  data_t data;
  
  enterprice::MultiCandleStream streams(5, [&](std::string const &code, unsigned long ts, double o, double h, double l, double c, double v){
    data.push_back(std::make_tuple(code, ts, o, h, l, c, v));
  });

  streams.data("OMG", 1000, 10, 1);
  streams.data("LOL", 1001, 12, 1);
  streams.data("LOL", 1005, 10, 1);
  streams.data("LOL", 1006, 9, 1);
  streams.data("OMG", 1006, 10, 1);
  
  assert( data[0]== data_t::value_type("LOL", 1001, 12, 12, 10, 10, 2) );
  assert( data[1]== data_t::value_type("OMG", 1000, 10, 10, 10, 10, 1) );
}

#endif

#endif

