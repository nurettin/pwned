all:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -std=c++11 -Wextra -Wall -pedantic -l gtest -l leveldb -l pthread -l snappy -Wfatal-errors
	@-./a.out
	@rm -r a.out data 
