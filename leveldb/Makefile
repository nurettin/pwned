all:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -std=c++11 -Wextra -Wall -pedantic -l pthread -l gtest -l leveldb -l snappy 
	@-./a.out
	@rm -r a.out data 
