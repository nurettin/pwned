#ifndef PWNED_PROGRESS_HPP
#define PWNED_PROGRESS_HPP

#include <iostream>
#include <ostream>
#include <iomanip>
#include <ctime>
#include <chrono>

namespace pwned { namespace progress {

using namespace std::chrono;

struct DurationParts
{
  hours h;
  minutes m;
  seconds s;
  milliseconds ms;

  DurationParts(milliseconds d)
  {
    h = duration_cast<hours>(d);
    d -= h;
    m = duration_cast<minutes>(d);
    d -= m;
    s = duration_cast<seconds>(d);
    d -= s;
    ms = duration_cast<milliseconds>(d);
  }
};

std::ostream &operator<< (std::ostream &o, DurationParts const &dp)
{
  return o<< std::setfill('0')
    << std::setw(2)<< dp.h.count()<< ":"
    << std::setw(2)<< dp.m.count()<< ":"
    << std::setw(2)<< dp.s.count()<< "."
    << std::setw(3)<< dp.ms.count();
}

template <typename clock>
struct ProgressClock
{
  uint64_t total_ticks, ticks;
  int total_bar_width;
  void(ProgressClock::* tick_ptr)();
  time_point<clock> start;
  int64_t elapsed_milliseconds;
  std::ostream &out;

  ProgressClock(uint64_t total_ticks, std::ostream &out= std::cout)
  : total_ticks(total_ticks)
  , ticks(0)
  , total_bar_width(50)
  , tick_ptr(&ProgressClock::first_tick)
  , start(clock::now())
  , elapsed_milliseconds(0)
  , out(out)
  {}

  void first_tick()
  {
    tick_ptr= &ProgressClock::continue_tick;
    out<< '\n';
    continue_tick();
  }

  void continue_tick()
  {
    ticks+= 1;
    elapsed_milliseconds= duration_cast<milliseconds>(clock::now()- start).count();
    print();
  }

  void tick()
  {
    (this->* tick_ptr)();
  }

  milliseconds estimate() const
  {
    int64_t average_tick_milliseconds= 0;
    if(ticks> 0)
      average_tick_milliseconds= elapsed_milliseconds/ ticks;
    uint64_t remaining_ticks= total_ticks- ticks;
    return milliseconds(average_tick_milliseconds* remaining_ticks);
  }

  std::string bar() const
  {
    int bar_width= total_bar_width* ticks/ total_ticks;
    int blank= total_bar_width- bar_width;
    std::string bar= "|";
    if(bar_width> 0)
      bar+= std::string(bar_width, '=');
    bar+= ">";
    if(blank> 0)
      bar+= std::string(blank, ' ');
    bar+= "|";
    return bar;
  }

  void print()
  {
    DurationParts estimate_parts(estimate());
    int64_t percent= 100* ticks/ total_ticks;
    DurationParts elapsed_parts((milliseconds(elapsed_milliseconds)));
    out<< "("
      << elapsed_parts <<") "
      << bar()<< " (% "<< std::setw(3)<< std::setfill(' ')
      << percent <<") ("
      << estimate_parts<< ")\r"<< std::flush;
  }

  ~ProgressClock()
  {
    out<< std::endl;
  }
};

typedef ProgressClock<std::chrono::high_resolution_clock> Progress;

} } // pwned progress

#endif

