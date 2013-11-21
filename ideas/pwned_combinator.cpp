struct Base
{
  virtual void use()= 0;
  virtual ~Base(){}
};

#include <functional>

template <typename T>
struct Derived: Base
{
  typedef std::function<void(Derived &)> Signal;
  T data;
  Signal signal;

  Derived(T const &data)
  : data(data)
  {}

  void use(){ signal(*this); }
};

#include <vector>
#include <iostream>

int main()
{
  std::vector<std::reference_wrapper<Base>> objects;
  Derived<int> d1(42);
  Derived<std::string> d2("omg");

  d1.signal= [](Derived<int> &d){ std::cout<< "d1's data: "<< d.data<< '\n'; };
  objects.push_back(d1);
  
  d2.signal= [](Derived<std::string> &d){ std::cout<< "d2's data: "<< d.data<< '\n'; };
  objects.push_back(d2);

  objects[0].get().use();
  objects[1].get().use();
};

