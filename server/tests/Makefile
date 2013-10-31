server:
	$(CXX) server.cpp -o server.out -std=c++11 -I ../../ -I ../../re2 -I ../../mongoose ../../mongoose/mongoose.c -L ../../re2/obj -l re2 -l pthread -l dl
test_routes:
	$(CXX) test_routes.cpp -o test_routes.out -std=c++11 -Wextra -Wall -I ../../re2 -L ../../re2/obj -l re2 -l pthread -l dl
test_named_groups:
	$(CXX) test_named_groups.cpp -o test_named_groups.out -std=c++11 -Wextra -Wall -I ../../ -I ../../mongoose -I ../../re2 -L ../../re2/obj -l re2 ../../mongoose/mongoose.c -l pthread -l dl
test_regex:
	$(CXX) test_regex.cpp -o test_regex.out -std=c++11 -Wextra -Wall -I ../../re2 -L ../../re2/obj -l re2 -l pthread -l dl
test_replace:
	$(CXX) test_replace.cpp -o test_replace.out -std=c++11 -Wextra -Wall -I ../../re2 -L ../../re2/obj -l re2 -l pthread -l dl
