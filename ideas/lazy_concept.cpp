#include <functional>
#include <vector>
#include <boost/optional.hpp>

struct LazyBase
{
  std::vector<std::reference_wrapper<LazyBase>> dependents;
  virtual void nullify()= 0;
  virtual ~LazyBase(){}
};

template <typename T>
struct Lazy: LazyBase
{
  void nullify(){ data= boost::none; }

  Lazy(){}
  Lazy(std::function<T ()> getter)
  : getter(getter)
  {}
  Lazy(T const &data)
  : data(data)
  {}
  Lazy(T const &data, boost::optional<std::function<T ()>> getter)
  : data(data)
  {}
  Lazy(std::function<T ()> getter, Lazy &)
  : getter(getter)
  {}

  void set(T const &data)
  {
    this-> data= data;
    for(auto &d: dependents)
      d.get().nullify();
  }

  T get()
  {
    if(!getter|| data)
      return *data;
    data= (*getter)();
    return *data;
  }
  
  Lazy &depends(LazyBase &other)
  {
    other.dependents.push_back(*this);
    return *this;
  }

private:
  boost::optional<T> data;
  boost::optional<std::function<T ()>> getter;
};

struct Laziness
{
  Lazy<int> first;
  Lazy<double> second;
  Lazy<double> total;

  Laziness()
  : total([&]{ return first.get()+ second.get(); }, total.depends(first).depends(second))
  {}
};

int main()
{
  Laziness lol;
  lol.first.set(42);
  lol.second.set(54);
  std::cout<< lol.total.get();
}

