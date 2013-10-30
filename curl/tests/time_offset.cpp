#include <ctime>
#include <iostream>

int main()
{
  std::time_t current_time;
  std::time(&current_time);
  struct std::tm *timeinfo = std::localtime(&current_time);
  long offset = timeinfo->tm_gmtoff/ 3600;
  std::cout<< offset<< std::endl;
}

