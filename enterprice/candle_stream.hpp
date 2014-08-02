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

#ifdef ENTERPRICE_IO_CANDLESTREAM_HPP_TEST

#include <tuple>
#include <vector>
#include <cassert>

int main()
{
  typedef std::vector<std::tuple<unsigned long, double, double, double, double, double>> data_t;
  data_t data;

  enterprice::CandleStream strm(5, [&](unsigned long ts, double o, double h, double l, double c, double v){
    data.push_back(std::make_tuple(ts, o, h, l, c, v));
  });

  strm.init_data(1000, 10, 1);
  strm.data(1003, 12, 2);
  strm.data(1007, 9, 1);
  strm.data(1011, 10, 3);
  strm.data(1021, 11, 4);
  strm.data(1022, 1, 4);
  strm.data(1023, 18, 4);
  strm.data(1024, 12, 4);
  strm.data(1026, 9, 4);
  strm.data(1036, 9, 4);

  assert( data[0]== data_t::value_type(1000, 10, 12, 10, 12, 3) );
  assert( data[1]== data_t::value_type(1005, 12, 9, 9, 9, 1) );
  assert( data[2]== data_t::value_type(1010, 9, 10, 10, 10, 3) );
  assert( data[3]== data_t::value_type(1015, 10, 10, 10, 10, 0) );
  assert( data[4]== data_t::value_type(1020, 10, 18, 1, 12, 16) );
  assert( data[5]== data_t::value_type(1025, 12, 9, 9, 9, 4) );
  assert( data[6]== data_t::value_type(1030, 9, 9, 9, 9, 0) );
}

#endif

#endif
