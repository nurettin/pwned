#include <iostream>
#include <ostream>
#include <iomanip>
#include <functional>
#include <chrono>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>

struct Fun
{
  std::string name;
  std::function<void()> fun;
  std::chrono::milliseconds duration;
  std::chrono::milliseconds total_duration;

  Fun(std::string const &name, std::function<void()> fun)
  : name(name)
  , fun(fun)
  , duration(0)
  , total_duration(0)
  {}
};
  
class Bm
{
  std::vector<Fun> funs;

  void bench(Fun const &fun)
  {
    funs.push_back(fun);
  }

  std::string report(int times) const
  {
    std::ostringstream report;
    report<< std::setprecision(2)<< std::fixed;
    std::vector<Fun*> pfuns;
    pfuns.reserve(funs.size());
    for(auto &fun: funs)
      pfuns.push_back(const_cast<Fun*>(&fun));
    std::sort(pfuns.begin(), pfuns.end(), [](Fun* f1, Fun* f2){ return f1-> total_duration< f2-> total_duration; });
    int max= pfuns.back()-> total_duration.count();
    report<< "Tests repeated: "<< times<< " times, averages:\n";
    int rank= 0;
    for(auto &fun: pfuns)
      report<< (++ rank)<< ". "<< fun-> name<< '\t'<< fun-> total_duration.count()/ times<< "ms\t"<< (100.* fun-> total_duration.count()/ max)<< "%\n";
    return report.str();
  }

public:
  template<typename... Args> 
  typename std::enable_if<std::is_constructible<Fun, Args...>::value>::type bench(Args&& ...args)
  {
    bench({std::forward<Args>(args)...}); 
  }

  void run(int times= 1, std::ostream &o= std::cout)
  {
    using namespace std::chrono;
    auto funs_size= funs.size();
    auto total_size= times* funs_size;
    #pragma omp parallel for
    for(std::size_t howmany= 0; howmany< total_size; ++ howmany)
    {
      milliseconds total(0);
      {
        auto fun= &funs[howmany% funs_size];
        auto t= high_resolution_clock::now();
        fun-> fun();
        fun-> duration= duration_cast<milliseconds>(high_resolution_clock::now()- t);
        fun-> total_duration+= fun-> duration;
        total+= fun-> duration;
      }
    }
    o<< report(times);
  }
};

int main()
{
  Bm bm;
  bm.bench("backward loop zero check", [](){
    for(uint64_t i= 100000000; i--;);
  });
  bm.bench("backward loop less check", [](){
    for(uint64_t i= 100000000; i> 0; i--);
  });
  bm.bench("forward loop greater check", [](){
    for(uint64_t i= 0; i< 100000000; ++ i);
  });
  bm.run(30);
}

