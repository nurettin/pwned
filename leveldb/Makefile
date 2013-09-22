all: run_tests

run_tests:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -Wextra -Wall -pedantic -l leveldb -l pthread -l gtest -l snappy -std=c++11
	@-./a.out
	@-rm -r a.out data 
