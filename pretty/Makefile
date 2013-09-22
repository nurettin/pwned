all: run_tests

run_tests:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -Wextra -Wall -pedantic -l pthread -l gtest -std=c++11
	@-./a.out
	@rm a.out
