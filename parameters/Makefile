all:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -std=c++11 -Wextra -Wall -pedantic -l gtest -l pthread 
	@-./a.out
	@rm a.out
