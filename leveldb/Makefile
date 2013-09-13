all: run_tests

run_tests:
	g++ tests.cpp -Wextra -Wall -pedantic -l leveldb -l pthread -l gtest -std=c++11
	-./a.out
	-rm -r a.out data 
