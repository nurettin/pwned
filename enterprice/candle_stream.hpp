#ifndef ENTERPRICE_IO_CANDLESTREAM_HPP
#define ENTERPRICE_IO_CANDLESTREAM_HPP

#include <functional>
#include <limits>
#include <iostream>

namespace pwned { namespace enterprice {

struct CandleStream
{
  typedef std::function<void(unsigned long, double, double, double, double, double)> block_t;
  unsigned long period;
  double pivot, prev_close, open, high, low, close, volume;
  block_t block;

  CandleStream(unsigned long period, block_t block)
  : period(period)
  , block(block)
  {
    reset_hlcv();
  }

  void init_data(unsigned long ts, double price, double amount)
  {
    pivot= ts;
    prev_close= open= high= low= close= price;
    volume= amount;
  }

  void data(unsigned long ts, double price, double amount)
  {
    if(ts> pivot+ period- 1)
    {
      call();
      prev_close= close;
      reset_hlcv();
      pivot+= period;
    }
    while(ts> pivot+ period- 1)
    {
      call_blank();
      pivot+= period;
    }
    if(ts< pivot+ period)
    {
      open= prev_close;
      if(price> high) high= price;
      if(price< low) low= price;
      close= price;
      volume+= amount;
    }
  }

  void reset_hlcv()
  {
    high= close= std::numeric_limits<double>::min();
    low= std::numeric_limits<double>::max();
    volume= 0;
  }

  void call() const
  {
    block(pivot, open, high, low, close, volume);
  }

  void call_blank() const
  {
    block(pivot, prev_close, prev_close, prev_close, prev_close, 0);
  }
};

} } // pwned::enterprice

#endif
