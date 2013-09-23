all:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -std=c++11 -Wextra -Wall -pedantic -l pthread -l gtest -l curl -l boost_filesystem -l boost_system -l htmlcxx -I ../hcxselect/src -L ../hcxselect/src -l hcxselect 
	@-./a.out
	@rm a.out
