all:
	$(CXX) server.cpp -o server.out -std=c++11 -Wextra -Wall -I ../../ -I ../../mongoose -I ../../re2 -L ../../re2/obj -l re2 ../../mongoose/mongoose.c -l pthread -l dl
test_routes:
	$(CXX) test_routes.cpp -o test_routes.out -std=c++11 -Wextra -Wall -I ../../ -I ../../mongoose -I ../../re2 -L ../../re2/obj -l re2 ../../mongoose/mongoose.c -l pthread -l dl
test_named_groups:
	$(CXX) test_named_groups.cpp -o test_named_groups.out -std=c++11 -Wextra -Wall -I ../../ -I ../../mongoose -I ../../re2 -L ../../re2/obj -l re2 ../../mongoose/mongoose.c -l pthread -l dl
