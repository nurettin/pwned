#include <string>
#include <map>

#include <re2/re2.h>

// original from here:
// http://codingways.blogspot.com/2013/05/how-to-use-named-group-feature-in.html
bool FindNamedGroups(const std::string &p_regex,const std::string &p_text,std::map<std::string,std::string> *p_group_value)
{
  p_group_value->clear();
  RE2 rx(p_regex);
  if(!rx.ok())
  {
    std::cerr << "Invalid Regular Expression :" << p_regex << std::endl;
    return false;
  }
  size_t named_grp_size =rx.NumberOfCapturingGroups();
  const std::map<std::string,int> &grp_to_idx=rx.NamedCapturingGroups();
  RE2::Arg args[10];
  std::string vars[10];
  const RE2::Arg * const p_args[10]={&args[0],&args[1],&args[2],&args[3],&args[4],&args[5],&args[6],&args[7],&args[8],&args[9]};
  int var_count=0;
 
  for(var_count=0;var_count<named_grp_size;var_count++)
    args[var_count]=&vars[var_count];
 
  re2::StringPiece sp_input(p_text);
  //after running following function. matched groups value  will be stored in p_args which point to args which point to vars!
  bool found= RE2::FindAndConsumeN(&sp_input,rx,p_args,named_grp_size);
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
