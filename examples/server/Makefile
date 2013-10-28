all:
	$(CXX) server.cpp -o server.out -std=c++11 -Wextra -Wall -I ../../ -I ../../mongoose -I ../../re2 -L ../../re2/obj -l re2 ../../mongoose/mongoose.c -l pthread -l dl
test:
	$(CXX) test_routes.cpp -o test_routes.out -std=c++11 -Wextra -Wall -I ../../ -I ../../mongoose -I ../../re2 -L ../../re2/obj -l re2 ../../mongoose/mongoose.c -l pthread -l dl
