#include <iostream>
#include <memory>
#include <typeinfo>
#include <vector>
#include <sstream>
#include <cxxabi.h>

namespace pwned { namespace type {

std::string demangle(char const* mangled)
{
  int status= 0;
  std::unique_ptr<char, void(*)(void*)> memo(abi::__cxa_demangle(mangled, 0, 0, &status), std::free);
  if(status== -1) { memo.release(); return mangled; }
  if(status!= 0) return mangled;
  return memo.get();
}

template <typename T>
std::string name()
{
  return demangle(typeid(T).name());
}

template <typename T>
std::string name(T const &t)
{
  return demangle(typeid(t).name());
}

} } // pwned type

struct FieldBase
{
  bool dirty;
  std::string type;
  std::string table;
  std::string column;

  FieldBase()
  : dirty(false)
  {}
  
  void initialize()
  {
    std::string type_name= pwned::type::name(*this);
    std::istringstream parse(type_name);
    std::string skip;
    bool ok= std::getline(parse, skip, '<') && 
      std::getline(parse, type, ',') &&
      parse.ignore(1);
    std::string token;
    // parse scopes
    std::getline(parse, token, ':') && parse.ignore(1);
    table+= token; 
    while(std::getline(parse, token, ':') && parse.ignore(1))
      table+= "_"+ token;
    column= token;
    // parse column name
    if(!column.empty())
      column.erase(std::prev(column.end()));
    if(!ok)
      throw std::runtime_error("Invalid type: "+ type_name+ " must be Field<type, Record::field>");
  }
  virtual void persist()= 0;
  virtual ~FieldBase() {}
};

namespace ActiveRecord {
  struct Base
  {
    int id;
    std::vector<FieldBase*> fields;
    void save()
    {
      for(auto &f: fields)
      {
        if(f-> dirty)
        {
          std::cout<< f-> type<< " / "<< f-> table<< " / "<< f-> column<< ", dirty: "<< std::boolalpha<< f-> dirty<< std::endl;
          f-> persist();
        }
      }
    }
  };
} // ActiveRecord

template <typename T, typename F>
struct Field: FieldBase
{
  T persisted_data, data;
  ActiveRecord::Base &model;
  
  Field(ActiveRecord::Base &model)
  : model(model)
  {
    model.fields.push_back(this);
    this-> initialize();
  }

  void persist()
  {
    persisted_data= data;
    dirty= false;
  }
  
  T operator= (T const &t)
  {
    dirty= persisted_data!= t;
    return data= t;
  }

  T operator ()() const
  {
    return data;
  }
};

#define attr_accessible(type, name) \
  enum name{}; Field<type, name> name= *this;

namespace wtf{ 

struct User: ActiveRecord::Base
{
  attr_accessible(std::string, login)
  attr_accessible(std::string, password)
};

}

int main()
{
  wtf::User u;
  u.login= "admin";
  std::cout<< u.login()<< std::endl;
  u.save();
}

