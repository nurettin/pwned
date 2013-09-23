all:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -std=c++11 -Wextra -Wall -pedantic -l pthread -l gtest
	@-./a.out
	@rm a.out
