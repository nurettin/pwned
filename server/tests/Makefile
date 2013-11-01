all: test_routes test_regex test_replace
test_routes:
	$(CXX) test_routes.cpp -o test_routes.out -std=c++11 -Wextra -Wall -I ../../re2 -L ../../re2/obj -l re2 -l pthread -l dl
test_regex:
	$(CXX) test_regex.cpp -o test_regex.out -std=c++11 -Wextra -Wall -I ../../re2 -L ../../re2/obj -l re2 -l pthread -l dl
test_replace:
	$(CXX) test_replace.cpp -o test_replace.out -std=c++11 -Wextra -Wall -I ../../re2 -L ../../re2/obj -l re2 -l pthread -l dl
