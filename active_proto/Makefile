all:
	@protoc person.proto --cpp_out=.
	@$(CXX) tests.cpp person.pb.cc -fprofile-arcs -ftest-coverage -std=c++11 -Wextra -Wall -pedantic -l gtest -l protobuf -l sqlite3 -l pthread
	@-./a.out
	@rm -r a.out db.sqlite person.pb.gcda person.pb.gcno
