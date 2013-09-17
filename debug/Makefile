all: run_tests

run_tests:
	@$(CXX) tests.cpp -Wextra -Wall -pedantic -l pthread -l gtest -std=c++11
	@-./a.out
	@rm a.out
