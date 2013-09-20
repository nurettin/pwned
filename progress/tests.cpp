#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

struct Progress
{
  uint64_t total_ticks, ticks;
  int total_bar_width, bar_width;
  void(Progress::* tick_ptr)();
  std::chrono::steady_clock::time_point start;

  Progress(uint64_t total_ticks)
  : total_ticks(total_ticks)
  , ticks(0)
  , total_bar_width(50)
  , bar_width(0)
  , tick_ptr(&Progress::first_tick)
  {}

  void first_tick()
  {
    tick_ptr= &Progress::continue_tick;
    start= std::chrono::steady_clock::now();
    std::cout<< '\n';
    continue_tick();
  }

  void continue_tick()
  {
    ticks+= 1;
    bar_width= total_bar_width* ticks/ total_ticks;
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
      average_tick_milliseconds= elapsed_milliseconds/ ticks;
    uint64_t remaining_ticks= total_ticks- ticks;
    auto estimate_ms= std::chrono::milliseconds(average_tick_milliseconds* remaining_ticks);
/*
    std::cout
      << "elapsed_milliseconds: "<< elapsed_milliseconds
      << "\naverage_tick_milliseconds: "<< average_tick_milliseconds
      << "\nremaining_ticks: "<< remaining_ticks
      << "\nestimate_ms.count(): "<< estimate_ms.count()
      << std::endl;*/
    return estimate_ms;
  }

  std::string bar() const
  {
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

  void print() const
  {
    auto d= estimate();
    auto h = std::chrono::duration_cast<std::chrono::hours>(d);
    d -= h;
    auto m = std::chrono::duration_cast<std::chrono::minutes>(d);
    d -= m;
    auto s = std::chrono::duration_cast<std::chrono::seconds>(d);
    d -= s;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(d);

    std::cout<< "(elapsed) "<< bar()<< " (percent) ("
      << std::setfill('0')
      << std::setw(2)<< h.count()<< ":"
      << std::setw(2)<< m.count()<< ":"
      << std::setw(2)<< s.count()<< "."
      << std::setw(3)<< ms.count()<< ")\r"
      << std::flush;
  }

};

#include <thread>

int main()
{
  Progress p(10);
  for(int n= 0; n< 10; ++ n)
  {
    p.tick();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

