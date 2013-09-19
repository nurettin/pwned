#ifndef PWNED_VIEW_HPP
#define PWNED_VIEW_HPP

#include <vector>
#include <functional>

namespace pwned { namespace view {

template <typename T>
struct View;

template <typename T>
struct ConstView;

template <typename T>
struct CopyView
{
  T t;

  CopyView(T const &t): t(t){}
  CopyView(CopyView &&v): t(v.t) {}
  CopyView(CopyView const &)= delete;
  CopyView &operator= (CopyView const &)= delete;

  template <typename F>
  View<T> each(F f)
  {
    for(auto &e: t) f(e);
    return View<T>(t);
  }

  template <typename F>
  CopyView map(F f)
  {
    T c;
    c.reserve(t.size());
    for(auto const &e: t) c.emplace_back(f(e));
    return CopyView(c);
  }

  template <typename F>
  CopyView select(F f)
  {
    T c;
    for(auto const &e: t) if(f(e)) c.emplace_back(e);
    return CopyView(c);
  }

  template <typename R, typename F>
  R reduce(F f, R r= R()) const
  {
    for(auto const &e: t) f(r, e);
    return r;
  }

  template <typename R>
  R sum(R r= R()) const
  {
    return reduce([](R &pr, typename T::value_type const &e){ pr+= e; }, r);
  }
};

template <typename T>
struct View
{
  T &t;

  View(T &t): t(t) {}
  View(View &&v): t(v.t) {}
  View(View const &)= delete;
  View &operator= (View const &)= delete;

  template <typename F>
  View &each(F f)
  {
    for(auto &e: t) f(e);
    return *this;
  }

  template <typename F>
  CopyView<T> map(F f)
  {
    T c;
    c.reserve(t.size());
    for(auto const &e: t) c.emplace_back(f(e));
    return CopyView<T>(c);
  }

  template <typename F>
  CopyView<T> select(F f)
  {
    T c;
    for(auto const &e: t) if(f(e)) c.emplace_back(e);
    return CopyView<T>(c);
  }

  template <typename R, typename F>
  R reduce(F f, R r= R()) const
  {
    for(auto const &e: t) f(r, e);
    return r;
  }

  template <typename R>
  R sum(R r= R()) const
  {
    return reduce([](R &pr, typename T::value_type const &e){ pr+= e; }, r);
  }
};

template <typename T>
struct ConstView
{
  T const &t;

  ConstView(T const &t): t(t) {}
  ConstView(ConstView &&v): t(v.t) {}
  ConstView(ConstView const &)= delete;
  ConstView &operator= (ConstView const &)= delete;

  template <typename F>
  ConstView each(F f)
  {
    for(auto const &e: t) f(e);
    return std::move(*this);
  }

  template <typename F>
  CopyView<T> map(F f)
  {
    T c;
    c.reserve(t.size());
    for(auto const &e: t) c.emplace_back(f(e));
    return CopyView<T>(c);
  }

  template <typename F>
  CopyView<T> select(F f)
  {
    T c;
    for(auto const &e: t) if(f(e)) c.emplace_back(e);
    return CopyView<T>(c);
  }

  template <typename R, typename F>
  R reduce(F f, R r= R()) const
  {
    for(auto const &e: t) f(r, e);
    return r;
  }

  template <typename R>
  R sum(R r= R()) const
  {
    return reduce([](R &pr, typename T::value_type const &e){ pr+= e; }, r);
  }
};

template <typename T>
struct RangeView 
{
  typedef typename T::value_type VT;
  T begin, end;

  RangeView(T begin, T end): begin(begin), end(end) {}
  RangeView(RangeView &&v): begin(v.begin), end(v.end) {}
  RangeView(RangeView const &)= delete;
  RangeView &operator= (RangeView const &)= delete;

  template <typename F>
  RangeView &each(F f)
  {
    for(T b= begin; b!= end; ++ b) f(*b);
    return *this;
  }

  template <typename F>
  RangeView &each(F f) const
  {
    for(T b= begin; b!= end; ++ b) f(*b);
    return *this;
  }

  template <typename F>
  CopyView<std::vector<VT>> map(F f)
  {
    std::vector<VT> c;
    for(T b= begin; b!= end; ++ b)
      c.emplace_back(f(*b));
    return CopyView<std::vector<VT>>(c);
  }

  template <typename F>
  CopyView<std::vector<VT>> select(F f)
  {
    std::vector<VT> c;
    for(T b= begin; b!= end; ++ b) if(f(*b)) c.emplace_back(*b);
    return CopyView<std::vector<VT>>(c);
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
    return reduce([](R &pr, typename T::value_type const &e){ pr+= e; }, r);
  }
};

template <typename T>
ConstView<T> view(T const &t)
{
  return ConstView<T>(t);
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

template <typename T>
struct Generate
{
  typedef T value_type;
  typedef std::function<bool(T &)> F;
  F f;
  T value;
  bool done;

  Generate(): f([](T &){ return true; }), value(T()), done(true) {}
  Generate(F f, T start= T()): f(f), value(start), done(false) {}

  bool operator!= (Generate const &g) { return done!= g.done; }
  
  Generate const &operator++ ()
  {
    if(!done) done= !f(value);
    return *this;
  }

  T operator* () const
  {
    return value;
  }
};

template <typename T, typename F= std::function<bool(T &)> >
std::pair<Generate<T>, Generate<T> > 
generate(F f, T const &t)
{
  return std::make_pair(Generate<T>(f, t), Generate<T>());
}

template <typename T>
std::pair<Generate<T>, Generate<T> >
generate(T const &begin, T const &end)
{
  return generate([&](T &i){ i+= 1; return i<= end; }, begin);
}

} } // pwned view

#endif

