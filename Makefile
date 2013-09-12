all: run_tests

run_tests:
	cd io && make && cd ..
	cd curl && make && cd ..
	cd leveldb && make && cd ..
	cd jsoncpp && make && cd ..

