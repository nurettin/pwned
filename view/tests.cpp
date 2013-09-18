#include <iostream>
#include <functional>
#include <algorithm>

template <typename T>
struct CopyView 
{
  typedef typename T::value_type VT;
  T t;

  CopyView(T const &t)
  : t(t)
  {}

  CopyView(CopyView const &)= delete;
  CopyView &operator= (CopyView const &)= delete;
  
  CopyView(CopyView &&v)
  : t(v.t)
  {}

  template <typename F>
  CopyView &each(F f)
  {
    for(auto &e: t) f(e);
    return *this;
  }

  template <typename F>
  CopyView const &each(F f) const
  {
    for(auto const &e: t) f(e);
    return *this;
  }

  template <typename F>
  CopyView map(F f)
  {
    T c;
    c.reserve(t.size());
    for(auto &e: t) c.emplace_back(f(e));
    return CopyView(c);
  }

  template <typename R, typename F>
  R reduce(F f, R r= R())
  {
    for(auto &e: t) f(r, e);
    return r;
  }

  template <typename R>
  R sum(R r= R())
  {
    return reduce([](R &pr, VT const &e){ pr+= e; }, r);
  }
};

template <typename T>
struct View 
{
  typedef typename T::value_type VT;
  T &t;

  View(T &t)
  : t(t)
  {}

  View(View const &)= delete;
  View &operator= (View const &)= delete;
  
  View(View &&v)
  : t(v.t)
  {}

  template <typename F>
  View &each(F f)
  {
    for(auto &e: t) f(e);
    return *this;
  }

  template <typename F>
  View const &each(F f) const
  {
    for(auto const &e: t) f(e);
    return *this;
  }

  template <typename F>
  CopyView<T> map(F f)
  {
    T c;
    c.reserve(t.size());
    for(auto &e: t) c.emplace_back(f(e));
    return CopyView<T>(c);
  }

  template <typename R, typename F>
  R reduce(F f, R r= R())
  {
    for(auto &e: t) f(r, e);
    return r;
  }

  template <typename R>
  R sum(R r= R())
  {
    return reduce([](R &pr, VT const &e){ pr+= e; }, r);
  }
};

template <typename T>
struct RangeView 
{
  typedef typename T::value_type VT;
  T begin, end;

  RangeView(T begin, T end)
  : begin(begin)
  , end(end)
  {}

  RangeView(RangeView const &)= delete;
  RangeView &operator= (RangeView const &)= delete;
  
  RangeView(RangeView &&v)
  : begin(v.begin)
  , end(v.end)
  {}

  template <typename F>
  RangeView &each(F f)
  {
    for(T b= begin; b!= end; ++ b) f(*b);
    return *this;
  }

  template <typename F>
  RangeView const &each(F f) const
  {
    for(T b= begin; b!= end; ++ b) f(*b);
    return *this;
  }

  template <typename F>
  CopyView<std::vector<T>> map(F f)
  {
    std::vector<T> c;
    c.reserve(end- begin);
    for(T b= begin; b!= end; ++ b)
      c.emplace_back(f(*b));
    return CopyView<std::vector<T>>(c);
  }

  template <typename R, typename F>
  R reduce(F f, R r= R())
  {
    for(T b= begin; b!= end; ++ b)
      f(r, *b);
    return r;
  }

  template <typename R>
  R sum(R r= R())
  {
    return reduce([](R &pr, VT const &e){ pr+= e; }, r);
  }
};

template <typename T>
CopyView<T> view(T const &t)
{
  return CopyView<T>(t);
}

template <typename T>
View<T> view(T &t)
{
  return View<T>(t);
}

template <typename T>
RangeView<T> view(T begin, T end)
{
  return RangeView<T>(begin, end);
}

int main()
{
  std::vector<int> v;
  v.reserve(30);
  v.emplace_back(1);
  v.emplace_back(2);
  v.emplace_back(3);
  view(v.begin(), v.end()).each([](int &i){ i+= 1; }).
    map([](int const &i){ return i* i; });
}

