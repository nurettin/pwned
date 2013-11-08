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
      parse.ignore(1) &&
      std::getline(parse, table, ':') &&
      parse.ignore(1) &&
      std::getline(parse, column, '>');
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
          std::cout<< pwned::type::name(*f)<< ", dirty: "<< std::boolalpha<< f-> dirty<< std::endl;
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
  enum name{}; Field<type, name> name

struct User: ActiveRecord::Base
{
  User()
  : login(*this)
  , password(*this)
  {}

  attr_accessible(std::string, login);
  attr_accessible(std::string, password);
};

int main()
{
  User u;
  u.login= "admin";
  std::cout<< u.login()<< std::endl;
  u.save();
}

