all:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -std=c++11 -Wextra -Wall -pedantic -I ../../ -l gtest -l pthread -l curl `pkg-config jsoncpp --cflags --libs`
	@-./a.out
	@rm a.out
