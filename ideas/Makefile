tbb_test.out: tbb_test.cpp
	$(CXX) tbb_test.cpp -std=c++11 -o tbb_test.out -l tbb
ecto1_test.out: ecto1_test.cpp
	g++ -std=c++11 ecto1_test.cpp -o ecto1_test.out -l curl -I ../.. `pkg-config jsoncpp --cflags --libs` && ./ecto1_test.out 2> err.txt && google-chrome shot.html
clean:
	rm -rf *.out
