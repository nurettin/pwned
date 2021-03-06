#ifndef ACTIVE_PROTO_HPP
#define ACTIVE_PROTO_HPP

#include <sqlite3.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <iostream>
#include <sstream>
#include <deque>

namespace pwned { namespace active_proto {

extern sqlite3* db;
sqlite3 *db= 0;

namespace 
{
  struct ActiveDB
  {
    static void trace(void*, char const* omg)
    {
      std::cout<< "sqlite3: "<< omg<< std::endl;
    }

    static void profile(void*, char const* omg, sqlite3_uint64 ms)
    {
      std::cout<< omg<< " : "<< (ms/ 1000000)/ 100.<< "ms"<< std::endl;
    }

    ActiveDB()
    {
      GOOGLE_PROTOBUF_VERIFY_VERSION;
      sqlite3_open("db.sqlite", &db);
      //sqlite3_trace(db, trace, 0);
      //sqlite3_profile(db, profile, 0);
    }

    ~ActiveDB()
    {
      sqlite3_close(db);
      google::protobuf::ShutdownProtobufLibrary();
    }
  } DB;
}

template <typename T>
struct Active
{
  static bool table_created;
  T older, newer;
  google::protobuf::Descriptor const* desc;
  google::protobuf::Reflection const* ref;

  Active()
  : newer(older)
  , desc(older.GetDescriptor())
  , ref(older.GetReflection())
  {
    if(table_created) return;
    execute_sql(table_sql());
    table_created= true;
  }

  T &get(){ return newer; }

  std::string table_sql()
  {
    std::string sql= "create table if not exists "+ desc-> name()+ "(";
    sql+= desc-> field(0)-> name();
    if(desc-> field(0)-> name()== "id")
      sql+= " INTEGER PRIMARY KEY";
    
    for(int d= 1; d< desc-> field_count(); ++ d)
    {
      std::string sql_type= proto_to_sql(desc-> field(d)-> type());
      if(sql_type.empty()) continue;
      sql+= ", "+ desc-> field(d)-> name()+ " "+ sql_type;
    }
    return sql+ ");";
  }

  std::string insert_sql()
  {
    std::string sql= "insert into "+ desc-> name()+ "(";
    std::ostringstream columns, values;
 
    auto f1= desc-> field(1);
    
    columns<< f1-> name();   
    values<< "?1";

    for(int d= 2; d< desc-> field_count(); ++ d)
    {
      auto f= desc-> field(d);
      if(f-> type()== google::protobuf::FieldDescriptor::TYPE_MESSAGE)
        continue;
      columns<< ", "<< f-> name();
      values<< ", ?"<< d;
    }

    sql+= columns.str()+ ") values("+ values.str()+ ");";
    return sql;
  }

  std::string update_sql(std::vector<google::protobuf::FieldDescriptor const*> dirty_fields)
  {
    if(dirty_fields.empty()) return "";

    std::ostringstream cat;
    cat<< "update "<< desc-> name()<< " set ";
    auto const &f0= dirty_fields.at(0);
    if(f0-> type()== google::protobuf::FieldDescriptor::TYPE_INT32)
      cat<< f0-> name()<< "= "<< ref-> GetInt32(newer, f0);
    else if(f0-> type()== google::protobuf::FieldDescriptor::TYPE_INT64)
      cat<< f0-> name()<< "= "<< ref-> GetInt64(newer, f0);
    else if(f0-> type()== google::protobuf::FieldDescriptor::TYPE_STRING)
      cat<< f0-> name()<< "= '"<< ref-> GetString(newer, f0)<< "'";
    else if(f0-> type()== google::protobuf::FieldDescriptor::TYPE_DOUBLE)
      cat<< f0-> name()<< "= "<< ref-> GetDouble(newer, f0);
    std::vector<decltype(dirty_fields)::value_type>(dirty_fields.begin()+ 1, dirty_fields.end()).swap(dirty_fields);
    for(auto &f: dirty_fields)
      if(f-> type()== google::protobuf::FieldDescriptor::TYPE_INT32)
        cat<< ", "<< f-> name()<< "= "<< ref-> GetInt32(newer, f);
      else if(f-> type()== google::protobuf::FieldDescriptor::TYPE_INT64)
        cat<< ", "<< f-> name()<< "= "<< ref-> GetInt64(newer, f);
      else if(f-> type()== google::protobuf::FieldDescriptor::TYPE_STRING)
        cat<< ", "<< f-> name()<< "= '"<< ref-> GetString(newer, f)<< "'";
      else if(f-> type()== google::protobuf::FieldDescriptor::TYPE_DOUBLE)
        cat<< ", "<< f-> name()<< "= "<< ref-> GetDouble(newer, f);
    cat<< " where id= ?1";
    return cat.str();
  }

  static std::string proto_to_sql(google::protobuf::FieldDescriptor::Type type)
  {
    switch(type)
    {
      case google::protobuf::FieldDescriptor::TYPE_INT32:
      case google::protobuf::FieldDescriptor::TYPE_INT64:
        return "INTEGER";
      case google::protobuf::FieldDescriptor::TYPE_STRING:
        return "TEXT";
      case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
        return "NUMERIC";
      default:
        break;
    }
    return "";
  }

  void execute_sql(std::string const &sql)
  {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, 0);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }

  void save()
  {
    if(!newer.has_id())
    {
      std::string sql= insert_sql();
      if(sql.empty()) return;

      sqlite3_stmt* stmt;
      sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, 0);
    
      for(int f= 1; f< desc-> field_count(); ++ f)
      {
        auto fp= desc-> field(f);
        if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_MESSAGE)
          continue;

        if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_INT32)
          sqlite3_bind_int(stmt, f, ref-> GetInt32(newer, fp));
        else if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_INT64)
          sqlite3_bind_int(stmt, f, ref-> GetInt64(newer, fp));
        else if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_STRING)
        {
          std::string str= ref-> GetString(newer, fp);
          sqlite3_bind_text(stmt, f, str.c_str(), str.size(), SQLITE_TRANSIENT);
        }
        else if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_DOUBLE)
          sqlite3_bind_double(stmt, f, ref-> GetDouble(newer, fp));
      }
        
      if(sqlite3_step(stmt) != SQLITE_DONE){
        std::cerr<< sqlite3_errmsg(db)<< '\n';
      } else {
        newer.set_id(sqlite3_last_insert_rowid(db));
      }
      sqlite3_finalize(stmt);
    }
    else
    {
      std::vector<google::protobuf::FieldDescriptor const*> dirty;
      for(int f= 0; f< desc-> field_count(); ++ f)
      {
        auto fp= desc-> field(f);
        if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_MESSAGE)
          continue;

        if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_INT32 && ref-> GetInt32(older, fp)!= ref-> GetInt32(newer, fp))
          dirty.push_back(fp);
        else if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_STRING && ref-> GetString(older, fp)!= ref-> GetString(newer, fp))
          dirty.push_back(fp);
        else if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_DOUBLE && ref-> GetDouble(older, fp)!= ref-> GetDouble(newer, fp))
          dirty.push_back(fp);
      }
      std::string sql= update_sql(dirty);
      if(sql.empty()) return;
      sqlite3_stmt* stmt;
      sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, 0);
      sqlite3_bind_int(stmt, 1, ref-> GetInt32(newer, desc-> field(0)));
      sqlite3_step(stmt); 
      sqlite3_finalize(stmt);
    }
    older= newer;
  }

  void load(int id)
  {
    std::string sql= "select * from "+ desc-> name()+ " where id= ?1 LIMIT 1;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);

    for(int f= 0; f< desc-> field_count(); ++ f)
    {
      auto fp= desc-> field(f);
      if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_MESSAGE)
        continue;

      if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_INT32)
      {
        ref-> SetInt32(&newer, fp, sqlite3_column_int(stmt, f));
        ref-> SetInt32(&older, fp, sqlite3_column_int(stmt, f));
      }
      else if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_STRING)
      {
        unsigned char const* text= sqlite3_column_text(stmt, f);
        std::string text_str;
        if(text!= 0) text_str= reinterpret_cast<char const*>(text);
        ref-> SetString(&newer, fp, text_str);
        ref-> SetString(&older, fp, text_str);
      }
      else if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_DOUBLE)
      {
        ref-> SetDouble(&newer, fp, sqlite3_column_double(stmt, f));
        ref-> SetDouble(&older, fp, sqlite3_column_double(stmt, f));
      }
    }
    newer.set_id(newer.id());
    older.set_id(older.id());
    sqlite3_finalize(stmt);
  }

  std::vector<T> all()
  {
    std::string sql= "select * from "+ desc-> name()+ ";";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, 0);
    std::vector<T> rows;
    T row;
    while(sqlite3_step(stmt)== SQLITE_ROW)
    {
      for(int f= 0; f< desc-> field_count(); ++ f)
      {
        auto fp= desc-> field(f);
        if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_MESSAGE)
          continue;

        if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_INT32)
          ref-> SetInt32(&row, fp, sqlite3_column_int(stmt, f));
        else if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_STRING)
        {
          unsigned char const* text= sqlite3_column_text(stmt, f);
          std::string text_str;
          if(text!= 0) text_str= reinterpret_cast<char const*>(text);
          ref-> SetString(&row, fp, text_str);
        }
        else if(fp-> type()== google::protobuf::FieldDescriptor::TYPE_DOUBLE)
          ref-> SetDouble(&row, fp, sqlite3_column_double(stmt, f));
      }
      rows.push_back(row);
    }
    sqlite3_finalize(stmt);
    return rows;
  } 

  int count()
  {
    std::string sql= "select count(1) from "+ desc-> name()+ ";";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, 0);
    sqlite3_step(stmt);
    int counter = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return counter;
  }

  void destroy()
  {
    std::string sql= "delete from "+ desc-> name()+ " where id= ?1;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, 0);
    sqlite3_bind_int(stmt, 1, ref-> GetInt32(newer, desc-> field(0)));
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
  
  void delete_all()
  {
    std::string sql= "delete from "+ desc-> name()+ ";";
    execute_sql(sql);
  }

  std::string make_fk_name(std::string const &model_name)
  {
    auto fk_name= model_name;
    fk_name[0]= std::tolower(fk_name[0]);
    fk_name+= "_id";
    return fk_name;
  }

  template <typename T2>
  T2 belongs_to()
  {
    T2 t2;
    auto t2_desc= t2.GetDescriptor();
    auto t2_ref= t2.GetReflection();
    auto t2_name= t2_desc-> name();
    auto fk_name= make_fk_name(t2_name);
    std::cout<< "foreign key name: "<< fk_name<< '\n';
    auto fk_field= desc-> FindFieldByName(fk_name);
    int fk_id= ref-> GetInt32(newer, fk_field);
    std::string sql= "select * from "+ t2_name+ " where id= ?1;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, 0);
    sqlite3_bind_int(stmt, 1, fk_id);

    T2 row;
    std::vector<T2> rows;
    sqlite3_finalize(stmt);
    return row;
  }
  /*
  template <typename T2>
  std::vector<T2> has_many()
  {
    auto this_fk_name= make_fk_name(desc-> name());
    T2 f;
    auto f_desc= f.GetDescriptor();
    auto f_name= f.GetDescriptor()-> name();
    auto f_ref= 
    std::string sql= "select * from "+ f_name+ " where "+ this_fk_name+ "= ?1;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, 0);
    sqlite3_bind_int(stmt, 1, ref-> GetInt23(newer, desc-> field(0));
    
    T2 row;
    std::vector<T2> rows;
    while(sqlite3_step(stmt)== SQLITE_ROW)
    {

    }

    sqlite3_finalize(stmt);
    return rows;
  }*/
};

template <typename T>
bool Active<T>::table_created(false);

} // active_proto
} // pwned

#endif 
