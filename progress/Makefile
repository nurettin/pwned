all:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -Wextra -Wall -std=c++11 -l pthread -l gtest -l boost_system -l boost_chrono -I .
	@./a.out
	@rm a.out
