all:
	@protoc db.proto --cpp_out=.
	@$(CXX) tests.cpp db.pb.cc -fprofile-arcs -ftest-coverage -std=c++11 -Wextra -Wall -pedantic -l gtest -l protobuf -l sqlite3 -l pthread
	@-./a.out
	@rm -r a.out db.sqlite db.pb.gcda db.pb.gcno
