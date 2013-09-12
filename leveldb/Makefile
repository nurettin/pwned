all: run_tests
run_tests:
	g++ tests.cpp -Wextra -Wall -pedantic -l leveldb -l pthread -l gtest
	./a.out
	-rm a.out
