#include <ctime>
#include <string>
#include <iostream>

std::string current_time_with_zone()
{
  char a[42];
  std::time_t current_time;
  std::time(&current_time);
  std::tm *timeinfo = std::localtime(&current_time);
  std::strftime(a, 42, "%c %z", timeinfo);
  return a;
}

int main()
{
  std::cout<< current_time_with_zone()<< std::endl;
}

