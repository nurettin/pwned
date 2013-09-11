all: run_tests

run_tests:
	g++ tests.cpp -Wextra -Wall -pedantic -l curl -l pthread -l gtest
	./a.out
	rm a.out
