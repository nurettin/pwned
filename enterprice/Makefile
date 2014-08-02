all:
	@$(CXX) -std=c++11 -O2 -Wextra -Wno-missing-field-initializers -Wall -Wpedantic -std=c++11 tests.cpp -l mysqlclient -l gtest -l pthread
	@-./a.out
	@rm a.out
