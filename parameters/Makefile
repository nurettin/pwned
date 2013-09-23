all:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -Wextra -Wall -pedantic -std=c++11 -l pthread -l gtest
	@./a.out
	@rm a.out
  
