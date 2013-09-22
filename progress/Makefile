all:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -Wextra -Wall -std=c++11 -l pthread -l gtest -I .
	@./a.out
	@rm a.out
