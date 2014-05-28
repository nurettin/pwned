all:
	@$(CXX) -std=c++11 -O2 -Wextra -Wall -Wpedantic -std=c++11 tests.cpp -DBOOST_LOG_DYN_LINK -l boost_log -l sqlite3 -l gtest -l pthread
	@-./a.out
	@rm a.out
