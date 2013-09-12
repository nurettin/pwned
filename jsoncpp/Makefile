all: run_tests

run_tests:
	g++ tests.cpp -Wextra -Wall -pedantic -std=c++11 `pkg-config jsoncpp --cflags --libs` -l pthread -l gtest
	-./a.out
	rm a.out
