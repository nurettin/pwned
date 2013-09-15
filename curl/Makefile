all: run_tests

run_tests:
	@g++ tests.cpp -Wextra -Wall -pedantic -l curl -l pthread -l gtest -l boost_filesystem -l boost_system -I ../hcxselect/src -L ../hcxselect/src -l hcxselect 
	@-./a.out
	@rm a.out
