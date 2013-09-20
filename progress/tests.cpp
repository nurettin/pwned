#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

struct Progress
{
  uint64_t total_ticks, ticks;
  void(Progress::* tick_ptr)();
  int64_t elapsed_msec;
  std::chrono::steady_clock::time_point start;

  Progress(uint64_t total_ticks)
  : total_ticks(total_ticks)
  , ticks(0)
  , tick_ptr(&Progress::first_tick)
  , elapsed_msec(0)
  {}

  void first_tick()
  {
    start= std::chrono::steady_clock::now();
    total_ticks+= 1;
    std::cout<< '\n';
    print();
    tick_ptr= &Progress::continue_tick;
  }

  void continue_tick()
  {
    total_ticks+= 1;
    print();
  }

  void tick()
  {
    (this->* tick_ptr)();
  }

  std::chrono::milliseconds
  estimate() const
  {
    int64_t elapsed_milliseconds= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()- start).count();
    int64_t average_tick_milliseconds= 0;
    if(elapsed_milliseconds> 0)
      average_tick_milliseconds= ticks/ elapsed_milliseconds;
    uint64_t remaining_ticks= total_ticks- ticks;
    return std::chrono::milliseconds(average_tick_milliseconds* remaining_ticks);
  }

  void print() const
  {
    auto estimate_ms= estimate();
    auto ms= estimate_ms % 1000;
    estimate_ms-= ms;
    auto s= std::chrono::duration_cast<std::chrono::seconds>(estimate_ms);
    estimate_ms-= s;
    auto m= std::chrono::duration_cast<std::chrono::minutes>(estimate_ms);
    estimate_ms-= m;
    auto h= std::chrono::duration_cast<std::chrono::hours>(estimate_ms);
    std::cout<< "(elapsed) |==============>                | (percent) ("
      << std::setfill('0')
      << std::setw(2)
      << h.count()<< ":"<< m.count()<< ":"<< s.count()<< "."
      << std::setw(3)
      << ms.count() 
      << ")\r";
  }
};

int main()
{
  Progress p(100);
  p.tick();
  p.tick();
}

