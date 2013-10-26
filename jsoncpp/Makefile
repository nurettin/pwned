all:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -std=c++11 -Wextra -Wall -pedantic -l gtest -l pthread `pkg-config jsoncpp --cflags --libs`
	@-./a.out
	@rm a.out
