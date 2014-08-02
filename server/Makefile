all:
	@$(CC) -c ../mongoose/mongoose.c -I ../mongoose
	@$(CXX) tests.cpp -fprofile-arcs -ftest-coverage -std=c++11 -Wextra -Wall -I ../hcxselect -I ../mongoose mongoose.o -I ../re2 -L ../re2/obj -l re2 -l curl -l gtest -l boost_filesystem -l boost_system -l boost_iostreams -l pthread -l dl
	@-./a.out
	@rm mongoose.*
	@rm a.out
