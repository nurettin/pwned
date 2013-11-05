all:
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -std=c++11 -Wextra -Wall -I ../hcxselect -I ../mongoose ../mongoose/mongoose.c -I ../re2 -L ../re2/obj -l re2 -l curl -l gtest -l boost_filesystem -l boost_system -l pthread -l dl
	@-./a.out
	@rm mongoose.*
	@rm a.out
