#ifndef MYSQL_HPP
#define MYSQL_HPP

#include <memory>
#include <string>
#include <stdexcept>
#include <mysql/mysql.h>
#include "memory.hpp"

namespace mysql {

bool check_mysql(MYSQL* p)
{
  return p!= 0 && mysql_errno(p)== 0;
}

void ensure_mysql(MYSQL* p)
{
  if(!check_mysql(p))
    throw std::runtime_error(mysql_error(p));
}

void ensure_query(MYSQL* p, int ret)
{
  ensure_mysql(p);
  if(ret!= 0)
    throw std::runtime_error(mysql_error(p));
}

std::unique_ptr<MYSQL, void(*)(MYSQL*)> connect(std::string host, std::string user, std::string passwd, std::string db, unsigned port= 3306, char const* fd= 0, unsigned long flags= CLIENT_COMPRESS| CLIENT_FOUND_ROWS)
{
  auto mysql= gc(mysql_init(0), mysql_close);
  ensure_mysql(mysql.get());
  ensure_mysql(mysql_real_connect(mysql.get(), host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, fd, flags));
  return mysql;
}

void query(MYSQL* p, std::string query, std::function<void(MYSQL_ROW)> block)
{
  ensure_query(p, mysql_query(p, query.c_str()));
  auto result= gc(mysql_use_result(p), mysql_free_result);
  while(auto row= mysql_fetch_row(result.get()))
  {
    ensure_mysql(p);
    block(row);
  }
}

} // mysql

#ifdef MYSQL_HPP_TEST

#include <cassert>

int main()
{
  auto db= mysql::connect("localhost", "root", "123456", "");
  assert( mysql::check_mysql(db.get()) );
}

#endif

#endif

