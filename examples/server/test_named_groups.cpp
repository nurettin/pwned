#include <string>
#include <map>
#include <iostream>
#include <vector>

#include <re2/re2.h>

// original from here:
// http://codingways.blogspot.com/2013/05/how-to-use-named-group-feature-in.html
bool find_named_groups(
  std::string const &p_regex
  , std::string const &p_text
  , std::map<std::string,std::string>* p_group_value)
{
  p_group_value-> clear();
  RE2 rx(p_regex);
  if(!rx.ok())
  {
    std::cerr << "Invalid Regular Expression :" << p_regex << std::endl;
    return false;
  }
  size_t named_grp_size =rx.NumberOfCapturingGroups();
  const std::map<std::string,int> &grp_to_idx=rx.NamedCapturingGroups();
  std::vector<RE2::Arg> args(named_grp_size);
  std::vector<std::string> vars(named_grp_size);
  std::vector<RE2::Arg*> p_args(named_grp_size);
  for(std::size_t i= 0; i< named_grp_size; ++ i)
    p_args[i]= &args[i];
  int var_count=0;
 
  for(var_count=0;var_count<named_grp_size;var_count++)
    args[var_count]=&vars[var_count];
 
  re2::StringPiece sp_input(p_text);
  //after running following function. matched groups value  will be stored in p_args which point to args which point to vars!
  bool found= RE2::FindAndConsumeN(&sp_input,rx,&p_args,named_grp_size);
  if(!found)
  {
    return false ;
  }
 
  std::map<std::string,int>::const_iterator iter_grps=grp_to_idx.cbegin();
  for(;iter_grps!=grp_to_idx.cend();++iter_grps)
  {
    (*p_group_value)[iter_grps->first]=vars[iter_grps->second-1];
  }
  return true;
  
}

int main()
{
}

