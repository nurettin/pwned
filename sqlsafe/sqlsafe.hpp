#ifndef PWNED_SQLSAFE_HPP
#define PWNED_SQLSAFE_HPP

#include <string>
#include <stdexcept>
#include <memory>
#include <functional>
#include <sqlite3.h>
#include <boost/log/trivial.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

namespace pwned{ namespace sqlsafe{

typedef std::unique_ptr<sqlite3, std::function<void(sqlite3*)>> db_type;
typedef std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt*)>> stmt_type;

bool check(sqlite3* db, int code)
{
  if(code== SQLITE_ROW|| code== SQLITE_OK) 
    return true;
  if(code== SQLITE_DONE)
    return false;
  sqlite3_mutex_enter(sqlite3_db_mutex(db));
  std::string err(sqlite3_errmsg(db));
  sqlite3_mutex_leave(sqlite3_db_mutex(db));
  if(code== SQLITE_MISUSE)
    err+= " SQLite misuse detected";
  BOOST_LOG_TRIVIAL(error)<< "SQLite Error: "+ err;
  return false;
}

bool check(db_type const &db, int code)
{
  return check(db.get(), code);
}

void ensure(sqlite3* db, int code)
{
  if(code== SQLITE_ROW|| code== SQLITE_DONE|| code== SQLITE_OK) 
    return;
  sqlite3_mutex_enter(sqlite3_db_mutex(db));
  std::string err(sqlite3_errmsg(db));
  sqlite3_mutex_leave(sqlite3_db_mutex(db));
  if(code== SQLITE_MISUSE)
    err+= " SQLite misuse detected";
  throw std::runtime_error("SQLite Error: "+ err);
}

void ensure(db_type const &db, int code)
{
  ensure(db.get(), code);
}

template <typename T, typename F>
std::unique_ptr<T, F> gc(T* ptr, F fun)
{
  return std::unique_ptr<T, F>(ptr, fun);
}

void trace(void*, char const* str)
{
  BOOST_LOG_TRIVIAL(trace)<< str;
}

std::unique_ptr<sqlite3, std::function<void(sqlite3*)>>
make_db(std::string const &file, int flags= SQLITE_OPEN_READWRITE| SQLITE_OPEN_CREATE| SQLITE_OPEN_FULLMUTEX| SQLITE_OPEN_WAL)
{
  sqlite3* p_db= 0;
  ensure(p_db, sqlite3_open_v2(file.c_str(), &p_db, flags, 0));
  std::function<void(sqlite3*)> fun= [](sqlite3* db){ check(db, sqlite3_close_v2(db)); };
  auto db= gc(p_db, fun);
  sqlite3_trace(db.get(), &trace, 0);
  return db;
}

std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt*)>>
make_stmt(std::unique_ptr<sqlite3, std::function<void(sqlite3*)>> const &db, std::string const &sql)
{
  sqlite3_stmt* p_stmt= 0;
  ensure(db.get(), sqlite3_prepare_v2(db.get(), sql.c_str(), sql.size(), &p_stmt, 0));
  std::function<void(sqlite3_stmt*)> fun= [&](sqlite3_stmt* st){ check(db.get(), sqlite3_finalize(st)); };
  auto stmt= gc(p_stmt, fun);
  return stmt;
}

struct stmt_set
{
  db_type const &db;
  stmt_type const &stmt;
  mutable int index= 1;

  stmt_set(db_type const &db, stmt_type const &stmt)
  : db(db), stmt(stmt)
  {}

  template <typename T>
  void operator()(T const &) const;
};

template <>
void stmt_set::operator()<int>(int const &t) const
{
  ensure(db, sqlite3_bind_int(stmt.get(), index, t));
  ++ index;
}

template <>
void stmt_set::operator()<double>(double const &t) const
{
  ensure(db, sqlite3_bind_double(stmt.get(), index, t));
  ++ index;
}

template <>
void stmt_set::operator()<std::string>(std::string const &t) const
{
  ensure(db, sqlite3_bind_text(stmt.get(), index, t.c_str(), t.size(), SQLITE_STATIC));
  ++ index;
}

template <typename... T>
void set(db_type const &db, stmt_type const &stmt, T&&... t)
{
  boost::fusion::for_each(std::forward_as_tuple<T>(t)..., stmt_set(db, stmt));
}

void reset(db_type const &db, stmt_type const &stmt)
{
  ensure(db, sqlite3_reset(stmt.get()));
}

template <typename... T>
struct scoped_setter
{
  db_type const &db;
  stmt_type const &stmt;

  scoped_setter(db_type const &db, stmt_type const &stmt, T&... tuple)
  : db(db)
  , stmt(stmt)
  {
    set(db, stmt, tuple...);    
  }

  ~scoped_setter(){ reset(db, stmt); }
};

template <typename... T>
scoped_setter<T&& ...> make_scoped_seter(db_type const &db, stmt_type const &stmt, T&&... tuple)
{
  return scoped_setter<T&& ...>(db, stmt, tuple...);
}

bool execute(db_type const &db, stmt_type const &stmt)
{
  return check(db.get(), sqlite3_step(stmt.get()));
}

bool execute(db_type const &db, std::string const &sql)
{
  return execute(db, make_stmt(db, sql));
}

template <typename... T>
bool execute(db_type const &db, stmt_type const &stmt, T&&... tuple)
{
  scoped_setter<T&&...> setter(db, stmt, tuple...);
  return execute(db, stmt);
}

struct stmt_get
{
  stmt_type const &stmt;
  mutable int index= 0;

  stmt_get(stmt_type const &stmt)
  : stmt(stmt)
  {}

  template <typename T>
  void operator()(T &t) const;
};
  
template <>
void stmt_get::operator()<int>(int &t) const
{
  t= sqlite3_column_int(stmt.get(), index);
  ++ index;
}

template <>
void stmt_get::operator()<double>(double &t) const
{
  t= sqlite3_column_double(stmt.get(), index);
  ++ index;
}

template <>
void stmt_get::operator()<char const*>(char const* &t) const
{
  t= reinterpret_cast<char const*>(sqlite3_column_text(stmt.get(), index));
  ++ index;
}

template <typename T>
bool get(db_type const &db, stmt_type const &stmt, T &t)
{
  bool ret= check(db, sqlite3_step(stmt.get()));
  if(ret) boost::fusion::for_each(t, stmt_get(stmt));
  return ret;
}

} // sqlsafe
} // pwned

#endif

